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

class MockLogger : public ILogger {
    MOCK_METHOD(void, Debug, (const std::string&), (const, override));
    MOCK_METHOD(void, Info, (const std::string&), (const, override));
    MOCK_METHOD(void, Warning, (const std::string&), (const, override));
    MOCK_METHOD(void, Error, (const std::string&), (const, override));

public:
    MockLogger(): MockLogger(false) {}
    explicit MockLogger(bool printOutput) {
        if (printOutput) {
            ON_CALL(*this, Debug).WillByDefault([](auto message) {
                std::cout << message << std::endl;
            });
            ON_CALL(*this, Info).WillByDefault([](auto message) {
                std::cout << message << std::endl;
            });
            ON_CALL(*this, Warning).WillByDefault([](auto message) {
                std::cout << message << std::endl;
            });
            ON_CALL(*this, Error).WillByDefault([](auto message) {
                std::cerr << message << std::endl;
            });
        }
    }
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
    testing::NiceMock<MockLogger> logger{}; // provide true argument to log to stdout
    example::MyAirlock airlock{example::AirlockDependencies{
        .doorInside = mockDoorInside,
        .doorOutside = mockDoorOutside,
        .vacuum = mockVacuum
    }, logger};

    auto insideOpenFuture = airlock.TransitionToInsideOpen();
    auto result = insideOpenFuture.get(); // Wait for future

    ASSERT_TRUE(result.has_value());
}

TEST_F(AirlockTest, TransitionToInsideOpen_FailureWhileEvacuating)
{
    testing::NiceMock<MockLogger> logger{}; // provide true argument to log to stdout
    example::MyAirlock airlock{example::AirlockDependencies{
        .doorInside = mockDoorInside,
        .doorOutside = mockDoorOutside,
        .vacuum = mockVacuum
    }, logger};

    // Arrange: Failure while evacuating
    EXPECT_CALL(mockVacuum, Evacuate()).WillOnce([this] {
        mockVacuum.TestSim_ChangeState(IVacuumHardware::VacuumState::Unknown);
    });

    auto insideOpenFuture = airlock.TransitionToInsideOpen();
    auto result = insideOpenFuture.get(); // Wait for future

    ASSERT_FALSE(result.has_value());
    ASSERT_STREQ("Vacuum state changed to unknown state: 0", result.error().c_str());
}
