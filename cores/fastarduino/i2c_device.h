//   Copyright 2016-2018 Jean-Francois Poilpret
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

/// @cond api

/**
 * @file 
 * I2C Device API.
 */
#ifndef I2C_DEVICE_HH
#define I2C_DEVICE_HH

#include "i2c.h"
#include "i2c_manager.h"

namespace i2c
{
	/**
	 * Bus condition (see [I2C](https://en.wikipedia.org/wiki/I%C2%B2C)) to be
	 * generated by `I2CDevice` read and write methods.
	 * This allows actual I2C devices to perform several reads and writes without
	 * "losing" the bus.
	 */
	enum class BusConditions : uint8_t
	{
		/** Generate no START condition at the beginning, no STOP condition at the end. */
		NO_START_NO_STOP = 0x00,
		/** Generate START condition at the beginning, but no STOP condition at the end. */
		START_NO_STOP = 0x01,
		/** Generate repeated START condition at the beginning, but no STOP condition at the end. */
		REPEAT_START_NO_STOP = 0x03,
		/** Generate START condition at the beginning and STOP condition at the end. */
		START_STOP = 0x05,
		/** Generate repeated START condition at the beginning and STOP condition at the end. */
		REPEAT_START_STOP = 0x07,
		/** Generate no START condition at the beginning, but generate STOP condition at the end. */
		NO_START_STOP = 0x04
	};

	/**
	 * Base class for all I2C devices.
	 * @tparam MODE_ the I2C mode for this device; this determines the `I2CManager` type
	 * that can manage this device.
	 * @sa i2c::I2CMode
	 * @sa i2c::I2CManager
	 */
	template<I2CMode MODE_ = I2CMode::Standard> class I2CDevice
	{
	public:
		/** the I2C mode for this device. */
		static constexpr const I2CMode MODE = MODE_;
		/** the type of `I2CManager` that can handle this device. */
		using MANAGER = I2CManager<MODE>;

	protected:
		/**
		 * Create a new I2C device. This constructor must be called by a subclass
		 * implementing an actua I2C device.
		 * @param manager the I2C Manager that is in charge of I2C bus
		 */
		I2CDevice(MANAGER& manager) : manager_{manager.handler()}
		{
		}

		/**
		 * Read a given number of bytes from an I2C device at the given address.
		 * I2C transmission may include START and STOP conditions, and device address,
		 * based on @p conditions argument.
		 * @param address I2C device address (only 7 MSB are used for the actual address
		 * the LSB is not used and should always be `0`); will be used only if
		 * @p conditions requires a START or repeated START condition
		 * @param data pointer to an array of bytes that will receive data received from
		 * this device; the array must have an allocated size of at least @p size bytes
		 * @param size the number of bytes to read from this device
		 * @param conditions which bus conditions should be transmitted on the bus; by 
		 * default, it indicates to transmit both START and STOP conditions, i.e. a full
		 * I2C frame. If you need to implement a more complex device protocol, e.g. mixing
		 * read and write or with several read operations in one frame, then you can use
		 * other conditions. Just do not forget that the first operation should always 
		 * include the START condition and the last operation the STOP condition.
		 * @return the I2C transmission status, `i2c::Status::OK` if the operation
		 * was successful, or any other `i2c::Status` code if it failed.
		 * 
		 * @sa i2c::BusConditions
		 * @sa i2c::Status
		 * @sa read(uint8_t, T&, BusConditions)
		 * @sa write()
		 */
		int read(uint8_t address, uint8_t* data, uint8_t size, BusConditions conditions = BusConditions::START_STOP)
		{
			bool ok = true;
			if (uint8_t(conditions) & 0x01)
				ok = (uint8_t(conditions) & 0x02 ? manager_.repeat_start() : manager_.start()) &&
					 manager_.send_slar(address);
			while (ok && --size) ok = manager_.receive_data(*data++);
			if (uint8_t(conditions) & 0x04)
			{
				ok = ok && manager_.receive_data(*data++, true);
				manager_.stop();
			}
			return manager_.status();
		}

		/**
		 * Read a given number of bytes from an I2C device at the given address and
		 * store it into the given type reference. The number of bytes to be received
		 * is determined by the type @p T.
		 * @note no `T` constructor or assignment operator will be called by this method,
		 * only storage will be affected.
		 * 
		 * I2C transmission may include START and STOP conditions, and device address,
		 * based on @p conditions argument.
		 * 
		 * @tparam T the type ot @p data, which will determine the number of bytes to receive
		 * 
		 * @param address I2C device address (only 7 MSB are used for the actual address
		 * the LSB is not used and should always be `0`); will be used only if
		 * @p conditions requires a START or repeated START condition
		 * @param data reference to a @p T object which content will receive data obtained from
		 * this device
		 * @param conditions which bus conditions should be transmitted on the bus; by 
		 * default, it indicates to transmit both START and STOP conditions, i.e. a full
		 * I2C frame. If you need to implement a more complex device protocol, e.g. mixing
		 * read and write or with several read operations in one frame, then you can use
		 * other conditions. Just do not forget that the first operation should always 
		 * include the START condition and the last operation the STOP condition.
		 * @return the I2C transmission status, `i2c::Status::OK` if the operation
		 * was successful, or any other `i2c::Status` code if it failed.
		 * 
		 * @sa i2c::BusConditions
		 * @sa i2c::Status
		 * @sa read(uint8_t, uint8_t*, uint8_t, BusConditions)
		 * @sa write()
		 */
		template<typename T> int read(uint8_t address, T& data, BusConditions conditions = BusConditions::START_STOP)
		{
			return read(address, (uint8_t*) &data, sizeof(T), conditions);
		}

		/**
		 * Write the given number of bytes to an I2C device at the given address
		 * 
		 * I2C transmission may include START and STOP conditions, and device address,
		 * based on @p conditions argument.
		 * 
		 * @param address I2C device address (only 7 MSB are used for the actual address
		 * the LSB is not used and should always be `0`); will be used only if
		 * @p conditions requires a START or repeated START condition
		 * @param data pointer to an array of bytes that contain data to be sent to
		 * this device; the array must have an allocated size of at least @p size bytes
		 * @param size the number of bytes to write to this device
		 * @param conditions which bus conditions should be transmitted on the bus; by 
		 * default, it indicates to transmit both START and STOP conditions, i.e. a full
		 * I2C frame. If you need to implement a more complex device protocol, e.g. mixing
		 * read and write or with several write operations in one frame, then you can use
		 * other conditions. Just do not forget that the first operation should always 
		 * include the START condition and the last operation the STOP condition.
		 * @return the I2C transmission status, `i2c::Status::OK` if the operation
		 * was successful, or any other `i2c::Status` code if it failed.
		 * 
		 * @sa i2c::BusConditions
		 * @sa i2c::Status
		 * @sa read()
		 * @sa write(uint8_t, const T&, BusConditions)
		 * @sa write(uint8_t, uint8_t, BusConditions)
		 */
		int write(uint8_t address, const uint8_t* data, uint8_t size,
				  BusConditions conditions = BusConditions::START_STOP)
		{
			bool ok = true;
			if (uint8_t(conditions) & 0x01)
				ok = (uint8_t(conditions) & 0x02 ? manager_.repeat_start() : manager_.start()) &&
					 manager_.send_slaw(address);
			while (ok && size--) ok = manager_.send_data(*data++);
			if (uint8_t(conditions) & 0x04) manager_.stop();
			return manager_.status();
		}
		
		/**
		 * Write, byte after byte, the content of @p data to an I2C device at the given 
		 * address. The number of bytes to be received is determined by type @p T.
		 * 
		 * I2C transmission may include START and STOP conditions, and device address,
		 * based on @p conditions argument.
		 * 
		 * @tparam T the type ot @p data, which will determine the number of bytes to send
		 * 
		 * @param address I2C device address (only 7 MSB are used for the actual address
		 * the LSB is not used and should always be `0`); will be used only if
		 * @p conditions requires a START or repeated START condition
		 * @param data reference to a @p T object which content will be sent to
		 * this device
		 * @param conditions which bus conditions should be transmitted on the bus; by 
		 * default, it indicates to transmit both START and STOP conditions, i.e. a full
		 * I2C frame. If you need to implement a more complex device protocol, e.g. mixing
		 * read and write or with several write operations in one frame, then you can use
		 * other conditions. Just do not forget that the first operation should always 
		 * include the START condition and the last operation the STOP condition.
		 * @return the I2C transmission status, `i2c::Status::OK` if the operation
		 * was successful, or any other `i2c::Status` code if it failed.
		 * 
		 * @sa i2c::BusConditions
		 * @sa i2c::Status
		 * @sa read()
		 * @sa write(uint8_t, const uint8_t*, uint8_t, BusConditions)
		 * @sa write(uint8_t, uint8_t, BusConditions)
		 */
		template<typename T>
		int write(uint8_t address, const T& data, BusConditions conditions = BusConditions::START_STOP)
		{
			return write(address, (const uint8_t*) &data, sizeof(T), conditions);
		}
		
		/**
		 * Write one data byte to an I2C device at the given address.
		 * 
		 * I2C transmission may include START and STOP conditions, and device address,
		 * based on @p conditions argument.
		 * 
		 * @param address I2C device address (only 7 MSB are used for the actual address
		 * the LSB is not used and should always be `0`); will be used only if
		 * @p conditions requires a START or repeated START condition
		 * @param data the byte to send to this device
		 * @param conditions which bus conditions should be transmitted on the bus; by 
		 * default, it indicates to transmit both START and STOP conditions, i.e. a full
		 * I2C frame. If you need to implement a more complex device protocol, e.g. mixing
		 * read and write or with several write operations in one frame, then you can use
		 * other conditions. Just do not forget that the first operation should always 
		 * include the START condition and the last operation the STOP condition.
		 * @return the I2C transmission status, `i2c::Status::OK` if the operation
		 * was successful, or any other `i2c::Status` code if it failed.
		 * 
		 * @sa i2c::BusConditions
		 * @sa i2c::Status
		 * @sa read()
		 * @sa write(uint8_t, const uint8_t*, uint8_t, BusConditions)
		 * @sa write(uint8_t, const T&, BusConditions)
		 */
		int write(uint8_t address, uint8_t data, BusConditions conditions = BusConditions::START_STOP)
		{
			bool ok = true;
			if (uint8_t(conditions) & 0x01)
				ok = (uint8_t(conditions) & 0x02 ? manager_.repeat_start() : manager_.start()) &&
					 manager_.send_slaw(address);
			ok = ok && manager_.send_data(data);
			if (uint8_t(conditions) & 0x04) manager_.stop();
			return manager_.status();
		}

	private:
		using HANDLER = typename MANAGER::HANDLER;
		HANDLER& manager_;
	};
};

#endif /* I2C_DEVICE_HH */
/// @endcond
