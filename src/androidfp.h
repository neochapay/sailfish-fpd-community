#ifndef ANDROIDFP_H
#define ANDROIDFP_H

#include <QObject>
#include "biometry.h"

class AndroidFP : public QObject
{
    Q_OBJECT
public:
    explicit AndroidFP(QObject *parent = nullptr);
    void enroll(uid_t user_id);
    void remove(uid_t finger);
    void cancel();
    void authenticate();
    void enumerate();
    void clear();

    Q_SLOT void enrollCallback(uint32_t finger, uint32_t remaining);
    Q_SLOT void removeCallback(uint32_t finger, uint32_t remaining);
    Q_SLOT void acquiredCallback(UHardwareBiometryFingerprintAcquiredInfo info);

signals:
    void failed(const QString& message);
    void succeeded(int fingerId); //After enrollment
    void removed(int finger); //0 for clear
    void authenticated(int fingerId);
    void enrollProgress(float progress); //Progress is 0..1
    void acquired(int info);

private:
    UHardwareBiometry m_biometry = nullptr;
    UHardwareBiometryParams fp_params;

    static void enrollresult_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *);
    static void acquired_cb(uint64_t, UHardwareBiometryFingerprintAcquiredInfo, int32_t, void *);
    static void authenticated_cb(uint64_t, uint32_t, uint32_t, void *);
    static void removed_cb(uint64_t, uint32_t, uint32_t, uint32_t, void *);
    static void enumerate_cb(uint64_t, uint32_t fingerId, uint32_t, uint32_t remaining, void *context);
    static void error_cb(uint64_t, UHardwareBiometryFingerprintError error, int32_t vendorCode, void *context);

    float m_enrollRemaining = 0.0;
    uint32_t m_removingFinger = 0;
};

#endif // ANDROIDFP_H
