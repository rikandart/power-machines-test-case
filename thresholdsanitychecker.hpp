#pragma once

// Qt
#include <QObject>
#include <QTimer>
// STL
#include <optional>
class thresholdSanityChecker : public QObject
{
    Q_OBJECT
public:
    enum class State : bool
    {
        PASSIVE,
        ACTIVE
    };
    Q_ENUM(State)
    Q_PROPERTY(double BA READ BA WRITE setBA NOTIFY BAChanged)
    Q_PROPERTY(double RMV READ RMV WRITE setRMV NOTIFY RMVChanged)
    Q_PROPERTY(int RDV READ RDV WRITE setRDV NOTIFY RDVChanged)
    Q_PROPERTY(State currentState READ currentState NOTIFY currentStateChanged)

    explicit thresholdSanityChecker(QObject *parent = nullptr);
    virtual ~thresholdSanityChecker() = default;
    void setBA(double val) noexcept
    {
        BA_ = val;
        emit BAChanged();
    }
    void setRMV(double val) noexcept
    {
        RMV_ = val;
        emit RMVChanged();
    }
    void setRDV(int msec) noexcept
    {
        RDVTimer_.setInterval(msec);
        emit RDVChanged();
    }
    double BA() const { return BA_.value_or(0.0); }
    double RMV() const { return RMV_.value_or(0.0); }
    int RDV() const { return RDVTimer_.interval(); }
    State currentState() const { return currentState_; }
    Q_INVOKABLE void checkAndSetState(const double Y) noexcept;
signals:
    void BAChanged();
    void RMVChanged();
    void RDVChanged();
    void currentStateChanged();
protected:
    enum class PassiveReturnRequirement : bool
    {
        NONE,
        HYSTERESIS
    };
    template<typename StartWithArgs, typename StopWithArgs>
    void checkPassiveState(const double Y, StartWithArgs startWithArgs, StopWithArgs stopWithArgs) noexcept;
    template<typename StartWithArgs, typename StopWithArgs>
    void checkActiveState(const double Y, const PassiveReturnRequirement returnRequirement, StartWithArgs startWithArgs, StopWithArgs stopWithArgs) noexcept;
    std::optional<double> BA_;
    std::optional<double> RMV_;
    QTimer RDVTimer_;
    State currentState_;
    State intendedState_;
};
