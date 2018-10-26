#ifndef PUFFIN_RANDOM_REAL_GENERATOR_H
#define PUFFIN_RANDOM_REAL_GENERATOR_H

#include <GL/glew.h>

#include <memory>
#include <random>

namespace puffin {
    class RandomRealGenerator {
    public:
        RandomRealGenerator(GLfloat min, GLfloat max) : 
            distribution_(min, max) {
            min_value_ = min;
            max_value_ = max;
        }

        GLfloat getMinValue() const {
            return min_value_;
        }

        GLfloat getMaxValue() const {
            return max_value_;
        }

        GLfloat getRandom() {
            return distribution_(random_engine_);
        }

    protected:
        std::default_random_engine random_engine_{};
        std::uniform_real_distribution<GLfloat> distribution_{};

        GLfloat min_value_{0.0f};
        GLfloat max_value_{0.0f};
    };

    using RandomRealGeneratorPtr = std::shared_ptr<RandomRealGenerator>;
} // namespace puffin

#endif // PUFFIN_RANDOM_REAL_GENERATOR_H