#pragma once

/* 5-values logic */
enum BitValue {
    ZERO, ONE, D, D_, X
};

class SignalValue
{

public:
    SignalValue(BitValue bitValue);
    BitValue bitValue() const;
    void setBitValue(const BitValue &bitValue);

    SignalValue operator&&(const SignalValue sigValue) const;
    SignalValue operator||(const SignalValue sigValue) const;
    inline bool operator==(const SignalValue sigValue) const { return m_bitValue == sigValue.bitValue(); }
    inline bool operator !=(const SignalValue &sigValue) const { return m_bitValue != sigValue.bitValue(); }
    SignalValue operator!();

private:
    BitValue m_bitValue;
};
