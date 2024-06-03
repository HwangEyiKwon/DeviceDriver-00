#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Project1/DeviceDriver.cpp"

using namespace testing;
using namespace std;

class MockFlashMemoryDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long), ());
	MOCK_METHOD(void, write, (long, unsigned char), ());
};

class DeviceDriverFixture : public testing::Test {
public:
	void makeObject() {
		mockFlashMemoryDevice = new MockFlashMemoryDevice();
		deviceDriverWithMock = new DeviceDriver(mockFlashMemoryDevice);
	}

	void deleteObject() {
		delete mockFlashMemoryDevice;
		delete deviceDriverWithMock;
	}

	MockFlashMemoryDevice* mockFlashMemoryDevice;
	DeviceDriver* deviceDriverWithMock;
};

TEST_F(DeviceDriverFixture, DeviceDriverReadCountTest) {
	makeObject();

	EXPECT_CALL(*mockFlashMemoryDevice, read).Times(5);
	deviceDriverWithMock->read(0xDEADDEAD);

	deleteObject();
}
TEST_F(DeviceDriverFixture, DeviceDriverReadExceptionTest) {
	makeObject();

	EXPECT_CALL(*mockFlashMemoryDevice, read).WillOnce(Return('A'))
											.WillOnce(Return('B'))
											.WillRepeatedly(Return('A'));
	try {
		deviceDriverWithMock->read(0xDEADDEAD);
		FAIL();
	}
	catch (exception e) {
		// PASS
	}

	deleteObject();
}
