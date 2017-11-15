#include "SignalValue.h"

SignalValue::SignalValue(BitValue bitValue)
    : m_bitValue(bitValue)
{
}

BitValue SignalValue::bitValue() const
{
    return m_bitValue;
}

void SignalValue::setBitValue(const BitValue &bitValue)
{
    m_bitValue = bitValue;
}

/**
 * AND Operator
 */
SignalValue SignalValue::operator&&(const SignalValue sigValue) const
{
    switch(m_bitValue)
    {
        case ZERO:
            return ZERO;
        case ONE:
            return sigValue;
        case D:
            switch (sigValue.bitValue())
            {
                case ZERO:
                    return ZERO;
                case ONE:
                    return D;
                case D:
                    return D;
                case D_:
                    return ZERO;
                case X:
                    return X;
            }
        case D_:
            switch (sigValue.bitValue())
            {
                case ZERO:
                    return ZERO;
                case ONE:
                    return D_;
                case D:
                    return ZERO;
                case D_:
                    return D_;
                case X:
                    return X;
            }
        case X:
            if (sigValue.bitValue() == ZERO)
                return ZERO;
            else
                return X;
        default:
            return X;
    }
}

SignalValue SignalValue::operator||(const SignalValue sigValue) const
{
    switch(m_bitValue)
    {
        case ZERO:
            return sigValue;
        case ONE:
            return ONE;
        case D:
            switch (sigValue.bitValue())
            {
                case ZERO:
                    return D;
                case ONE:
                    return ONE;
                case D:
                    return D;
                case D_:
                    return ONE;
                case X:
                    return X;
            }
        case D_:
            switch (sigValue.bitValue())
            {
                case ZERO:
                    return D_;
                case ONE:
                    return ONE;
                case D:
                    return ONE;
                case D_:
                    return D_;
                case X:
                    return X;
            }
        case X:
            if (sigValue.bitValue() == ONE)
                return ONE;
            else
                return X;
        default:
            return X;
    }
}

SignalValue& SignalValue::operator~()
{
    switch(m_bitValue)
    {
        case ZERO:
            m_bitValue = ONE;
        case ONE:
            m_bitValue = ZERO;
        case D:
            m_bitValue = D_;
        case D_:
            m_bitValue = D;
        case X:
            m_bitValue = X;
    }
    return *this;
}
