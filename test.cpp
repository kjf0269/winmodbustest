#include "pch.h"
#include "../winmodbus/MBAPHeader.h"
#include "../winmodbus/ReadCoilsRequest.h"


TEST(MBAPHeader, MBAPHeaderSize) {
    MBAPHeader* ts = new MBAPHeader(0x1234, 100, 1);
    EXPECT_EQ(ts->getMBAPHeaderLength(), 7);
    delete ts;
}

TEST(MBAPHeader, MBAPContents) {
    MBAPHeader* ts = new MBAPHeader(0x1234, 0x100, 0x42);
    uint8_t buffer[8];
    int result = ts->copyHeader(buffer);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(buffer[0], 0x12);
    EXPECT_EQ(buffer[1], 0x34);
    EXPECT_EQ(buffer[2], 0x00);
    EXPECT_EQ(buffer[3], 0x00);
    EXPECT_EQ(buffer[4], 0x01);
    EXPECT_EQ(buffer[5], 0x00);
    EXPECT_EQ(buffer[6], 0x42);
}

TEST(MBAPHeader, RecvMBAPDecode) {
    uint8_t msg[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };
    MBAPHeader* ts = new MBAPHeader(msg);

    EXPECT_EQ(ts->getTransactionId(), 0x0123);
    EXPECT_EQ(ts->getProtocolId(), 0x4567);
    EXPECT_EQ(ts->getLength(), 0x89ab);
    EXPECT_EQ(ts->getUnitId(), 0xcd);
}

TEST(PDU, ReadCoilsRequest) {
    ReadCoilsRequest* ts = new ReadCoilsRequest();
    int result = ts->setQuantity(0x0120);
    EXPECT_EQ(result, 0);
    result = ts->setStartAddress(0x2010);
    EXPECT_EQ(result, 0);

    uint8_t buffer[2000];
    unsigned int length = ts->getMessage(buffer);
    EXPECT_EQ(5, length);
    EXPECT_EQ(1, buffer[0]);
    EXPECT_EQ(0x20, buffer[1]);
    EXPECT_EQ(0x10, buffer[2]);
    EXPECT_EQ(0x01, buffer[3]);
    EXPECT_EQ(0x20, buffer[4]);
    delete ts;
}

TEST(PDU, ReadCoilsRequestError) {
    ReadCoilsRequest* ts = new ReadCoilsRequest();
    int result = ts->setQuantity(0);
    EXPECT_EQ(result, -1);
    result = ts->setQuantity(2001);
    EXPECT_EQ(result, -1);
    result = ts->setQuantity(1);
    EXPECT_EQ(result, 0);
    result = ts->setQuantity(2000);
    EXPECT_EQ(result, 0);
    delete ts;
}

TEST(PDU, ReadCoilsResponse) {

}