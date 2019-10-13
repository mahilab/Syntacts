#include <Syntacts/Syntacts.hpp>

using namespace tact;

class Osc : public Source
{
public:
    Osc(std::shared_ptr<Source> frequency) : m_frequency(std::move(frequency))
    {
    }

    Osc(float frequency) : m_frequency(std::make_shared<Scalar>(frequency))
    {
    }

    virtual float sample(float t) override
    {
        return std::sin(2 * 3.14f * m_frequency->sample(t) * t);
    }

private:
    std::shared_ptr<Source> m_frequency;
};

class Ramp : public Source
{
public:
    Ramp(float initial, float rate) : m_initial(initial),
                                      m_rate(rate)
    {
    }

    Ramp(float initial, float final, float span) : m_initial(initial),
                                                   m_rate((final - initial) / span)
    {
    }

    virtual float sample(float t) override
    {
        return m_initial + m_rate * t;
    }

private:
    float m_initial;
    float m_rate;
};

int main(int argc, char const *argv[])
{
    auto rmp = std::make_shared<Ramp>(440.0f, 880.0f, 6.0f);
    auto osc = std::make_shared<Osc>(rmp);
    auto env = std::make_shared<ASR>(2.0f, 2.0f, 2.0f, 1.0f);
    auto cue = std::make_shared<Cue>();
    cue->chain(osc);
    // cue->chain(rmp);
    cue->setEnvelope(env);
    save(cue, "my_cue.wav");
    return 0;
}
