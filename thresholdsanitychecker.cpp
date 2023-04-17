#include "thresholdsanitychecker.hpp"
#include <QDebug>

thresholdSanityChecker::thresholdSanityChecker(QObject *parent)
    : QObject{parent}, currentState_{State::PASSIVE}
{
    RDVTimer_.callOnTimeout([this]() mutable {
        currentState_ = intendedState_;
        emit currentStateChanged();
        RDVTimer_.stop();
    });
}

void thresholdSanityChecker::checkAndSetState(const double Y) noexcept
{
    static PassiveReturnRequirement returnRequirementGlob = PassiveReturnRequirement::NONE;
    static auto startTimerWithStateAndReturnRequirement = [this](State state, PassiveReturnRequirement returnRequirement) mutable
    {
        RDVTimer_.start();
        intendedState_ = state;
        returnRequirementGlob = returnRequirement;
    };
    static auto stopTimerWithState = [this](State state) mutable
    {
        RDVTimer_.stop();
        intendedState_ = state;
    };

    switch(currentState_)
    {
    case State::PASSIVE:
        checkPassiveState(Y, startTimerWithStateAndReturnRequirement, stopTimerWithState);
    break;

    case State::ACTIVE:
        checkActiveState(Y, returnRequirementGlob, startTimerWithStateAndReturnRequirement, stopTimerWithState);
    break;
    }
}

template<typename StartWithArgs, typename StopWithArgs>
void thresholdSanityChecker::checkPassiveState(const double Y, StartWithArgs startWithArgs, StopWithArgs stopWithArgs) noexcept
{
    const double BAValue = BA_.value_or(0.0);
    const double RMVValue = RMV_.value_or(0.0);
    if(Y > BAValue+RMVValue && !RDVTimer_.isActive())
    {
        startWithArgs(State::ACTIVE, PassiveReturnRequirement::NONE);
    }
    else if(Y > BAValue && !RDVTimer_.isActive())
    {
        startWithArgs(State::ACTIVE, PassiveReturnRequirement::HYSTERESIS);
    }
    else if(Y <= BAValue && RDVTimer_.isActive())
    {
        stopWithArgs(State::PASSIVE);
    }
}

template<typename StartWithArgs, typename StopWithArgs>
void thresholdSanityChecker::checkActiveState(const double Y, const PassiveReturnRequirement returnRequirement, StartWithArgs startWithArgs, StopWithArgs stopWithArgs) noexcept
{
    const double BAValue = BA_.value_or(0.0);
    const double RMVValue = RMV_.value_or(0.0);
    double threshold;
    switch(returnRequirement)
    {
    case PassiveReturnRequirement::NONE:
        threshold = BAValue;
    break;

    case PassiveReturnRequirement::HYSTERESIS:
        threshold = BAValue-RMVValue;
    break;
    }

    if(Y <= threshold && !RDVTimer_.isActive())
    {
        startWithArgs(State::PASSIVE, returnRequirement);
    }
    else if(Y > threshold && RDVTimer_.isActive())
    {
        stopWithArgs(State::ACTIVE);
    }
}
