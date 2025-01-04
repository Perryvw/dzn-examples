#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "airlock.hpp"

class MockDoor : public IDoorHardware
{
public:
    MOCK_METHOD(void, Open, (), (override));
    MOCK_METHOD(void, Close, (), (override));
    MOCK_METHOD(void, ObserveStateChanged, (std::function<void(DoorState)>), (override));

    MockDoor() {
        ON_CALL(*this, Open).WillByDefault([this]() {
            for (auto& observer : observers) {
                observer(DoorState::Open);
            }
        });
        ON_CALL(*this, Close).WillByDefault([this]() {
            for (auto& observer : observers) {
                observer(DoorState::Closed);
            }
        });
        ON_CALL(*this, ObserveStateChanged).WillByDefault([this](auto observer) {
            observers.emplace_back(observer);
        });
    }

    std::vector<std::function<void(DoorState)>> observers;
};

class MockVacuum : public IVacuumHardware
{
public:
    MOCK_METHOD(void, Evacuate, (), (override));
    MOCK_METHOD(void, Vent, (), (override));
    MOCK_METHOD(void, ObserveStateChanged, (std::function<void(VacuumState)>), (override));

    MockVacuum() {
        ON_CALL(*this, Evacuate).WillByDefault([this]() {
            // Immediately finish
            TestSim_ChangeState(VacuumState::Vacuum);
        });
        ON_CALL(*this, Vent).WillByDefault([this]() {
            // Immediately finish
            TestSim_ChangeState(VacuumState::Ambient);
        });
        ON_CALL(*this, ObserveStateChanged).WillByDefault([this](auto observer) {
            observers.emplace_back(observer);
        });
    }

    void TestSim_ChangeState(IVacuumHardware::VacuumState state) {
        for (auto& observer : observers) {
            observer(state);
        }
    }

    std::vector<std::function<void(VacuumState)>> observers;
};

class AirlockTest : public testing::Test
{
protected:
    testing::NiceMock<MockDoor> mockDoorInside;
    testing::NiceMock<MockDoor> mockDoorOutside;
    testing::NiceMock<MockVacuum> mockVacuum;
};

TEST_F(AirlockTest, TransitionToInsideOpen_Ok)
{
    example::MyAirlock airlock{example::AirlockDependencies{
        .doorInside = mockDoorInside,
        .doorOutside = mockDoorOutside,
        .vacuum = mockVacuum
    }};

    auto insideOpenFuture = airlock.TransitionToInsideOpen();
    auto result = insideOpenFuture.get(); // Wait for future

    ASSERT_TRUE(result.has_value());
}

TEST_F(AirlockTest, TransitionToInsideOpen_FailureWhileEvacuating)
{
    example::MyAirlock airlock{example::AirlockDependencies{
        .doorInside = mockDoorInside,
        .doorOutside = mockDoorOutside,
        .vacuum = mockVacuum
    }};

    // Arrange: Failure while evacuating
    EXPECT_CALL(mockVacuum, Evacuate()).WillOnce([this] {
        mockVacuum.TestSim_ChangeState(IVacuumHardware::VacuumState::Unknown);
    });

    auto insideOpenFuture = airlock.TransitionToInsideOpen();
    auto result = insideOpenFuture.get(); // Wait for future

    ASSERT_FALSE(result.has_value());
    ASSERT_STREQ("Vacuum state changed to unknown state: 0\n", result.error().c_str());
}
