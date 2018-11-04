/*
* Puffin OpenGL Engine
* Created by: Sebastian 'qbranchmaster' Tabaka
*/

#ifndef PUFFIN_RANDOM_GENERATOR_HPP
#define PUFFIN_RANDOM_GENERATOR_HPP

#include <GL/glew.h>

#include <memory>
#include <random>

namespace puffin {
    class RandomGenerator {
    public:
        RandomGenerator(GLfloat min, GLfloat max) :
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

    using RandomGeneratorPtr = std::shared_ptr<RandomGenerator>;
} // namespace puffin

#endif // PUFFIN_RANDOM_GENERATOR_HPP