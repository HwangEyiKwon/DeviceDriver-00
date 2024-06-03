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
		mockFlashMemoryDevice = new NiceMock<MockFlashMemoryDevice>();
		deviceDriverWithMock = new DeviceDriver(mockFlashMemoryDevice);
	}

	void deleteObject() {
		delete mockFlashMemoryDevice;
		delete deviceDriverWithMock;
	}

	NiceMock<MockFlashMemoryDevice>* mockFlashMemoryDevice;
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
TEST_F(DeviceDriverFixture, DeviceDriverNormalReadTest) {

	makeObject();

	EXPECT_CALL(*mockFlashMemoryDevice, read).WillRepeatedly(Return('A'));
	EXPECT_THAT(deviceDriverWithMock->read(0xDEADDEAD), Eq(int('A')));

	deleteObject();
}

TEST_F(DeviceDriverFixture, DeviceDriverWriteExceptionTest) {

	makeObject();

	EXPECT_CALL(*mockFlashMemoryDevice, read).WillRepeatedly(Return('A'));
	try {
		deviceDriverWithMock->write(0xDEADDEAD, 0xDEAD);
		FAIL();
	}
	catch (exception e) {
		// PASS
	}

	deleteObject();
}

TEST_F(DeviceDriverFixture, DeviceDriverNormalWriteTest) {

	makeObject();

	EXPECT_CALL(*mockFlashMemoryDevice, read).WillOnce(Return((unsigned char)0xFF))
											.WillRepeatedly(Return((unsigned char)0xA));
	deviceDriverWithMock->write(0xDEADDEAD, 0xA);

	EXPECT_THAT(deviceDriverWithMock->read(0x0), Eq(0xA));

	deleteObject();
}