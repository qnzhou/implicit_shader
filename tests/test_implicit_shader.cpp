#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <implicit_shader/Duchon.h>
#include <implicit_shader/primitives.h>

#include <implicit_functions.h>

#include <array>

namespace {
void finite_difference_check(
    implicit_shader::ImplicitFunction& fn,
    const implicit_shader::Point& p,
    float d = 1e-3,
    float error = 1e-2)
{
    // clang-format off
    std::array<implicit_shader::Scalar, 28> buffer = {
        p[0], p[1], p[2], 0,
        p[0] - d, p[1], p[2], 0,
        p[0] + d, p[1], p[2], 0,
        p[0], p[1] - d, p[2], 0,
        p[0], p[1] + d, p[2], 0,
        p[0], p[1], p[2] - d, 0,
        p[0], p[1], p[2] + d, 0
    };
    // clang-format on

    fn.evaluate(buffer);

    const float dx = (buffer[11] - buffer[7]) / (2 * d);
    const float dy = (buffer[19] - buffer[15]) / (2 * d);
    const float dz = (buffer[27] - buffer[23]) / (2 * d);

    float x_err = dx - buffer[0];
    float y_err = dy - buffer[1];
    float z_err = dz - buffer[2];

    float l = std::sqrt(dx * dx + dy * dy + dz * dz);
    float err = std::sqrt(x_err * x_err + y_err * y_err + z_err * z_err);
    REQUIRE(err <= error * l);
}

auto vipss_data()
{
    // clang-format off
    std::array<float, 25 * 3> control_points = {
        -0.032937999814748764, -0.44808998703956604, -0.20285199582576752,
        0.36215001344680786, -0.5138090252876282, 0.3453710079193115,
        -0.33134201169013977, -0.5005689859390259, 0.33508598804473877,
        -0.20052699744701385, -0.5373740196228027, -0.3636530041694641,
        0.3809880018234253, -0.521619975566864, -0.27469199895858765,
        -0.3067519962787628, -0.5032169818878174, -0.31551700830459595,
        -0.3286600112915039, -0.504817008972168, 0.009344999678432941,
        0.3748619854450226, -0.5167909860610962, -0.05943100154399872,
        0.2386849969625473, -0.44690999388694763, 0.24606600403785706,
        0.04156399890780449, -0.49768200516700745, 0.35078901052474976,
        -0.1845729947090149, -0.4497759938240051, 0.2515510022640228,
        -0.1647450029850006, -0.4673439860343933, -0.279772013425827,
        0.24178700149059296, -0.45748400688171387, -0.2486409991979599,
        -0.32465699315071106, -0.5070750117301941, -0.1992270052433014,
        0.22840599715709686, -0.5201280117034912, -0.349932998418808,
        0.17870000004768372, -0.44439300894737244, 0.14688999950885773,
        0.36926400661468506, -0.5213289856910706, -0.3388180136680603,
        0.37018999457359314, -0.5112159848213196, 0.16212299466133118,
        0.2067669928073883, -0.45385101437568665, -0.1727990061044693,
        0.2356259971857071, -0.446289986371994, -0.020424000918865204,
        0.04234499856829643, -0.5018489956855774, -0.3371509909629822,
        -0.33829501271247864, -0.505262017250061, 0.21567299962043762,
        -0.20809000730514526, -0.49874401092529297, 0.35121798515319824,
        -0.12110999971628189, -0.4454120099544525, 0.14874599874019623,
        0.19960300624370575, -0.4915440082550049, 0.3480679988861084
    };

    std::array<float, 25 * 4> coeff_a = {
        2.4740128434376953, -2.553941889215828, -8.033120267963975,  12.416739582671926,
        13.789505334358951, -13.39674516050803, -2.389331895248703, 0.8952914633269015,
        -13.501506214927561, -0.05799279366371746, -14.360154650328642, -9.2144628781077,
        -23.11952231989834, 10.123585274007942, 14.214967697582756, 7.388153497604954,
        -14.805784594972913, 5.492102879830876, 15.099169205974572, -7.252061440988718,
        -3.379991720075498, 8.350582400321246, 10.222338645673599, 4.873123879311303,
        6.725043121797929, -0.0011981931961453987, -0.05546426483130996, 0.029622371283877305,
        -0.0011714166862672815, -0.02339469054926046, 0.0699829625036836, -0.00378570150310812,
        0.008309892546944243, -0.01989122916875552, -0.00009888006263798765, 0.023170712201192052,
        0.016546570546870167, -0.03231286043815298, 0.016698361151766716, 0.004807118263180241,
        -0.0011518022000379727, -0.0974819581942843, -0.010818383176706583, -0.004808308534442715,
        -0.015233442794490903, -0.0013463294084890712, 0.015234556042550342, 0.0016621397106665325,
        0.0011443900334884433, 0.00028716531997055594, -0.030729009911737278, -0.13171618252946088,
        -0.17222479091256754, -0.029133434472745502, -0.04302814464253011, -0.33997597288405956,
        0.00963238086854553, 0.012172797218923911, -0.18180422292721127, -0.013511784880998454,
        -0.16280990032457093, -0.17376787346910297, -0.27790156462114896, -0.04112297658943231,
        -0.07673242777277985, -0.07916096308570512, -0.2974112221850787, -0.01575900174423878,
        -0.08168412983552965, -0.06765886861231246, -0.038778439933703966, -0.03775987724660256,
        -0.0518698728117481, -0.035955532321255076, -0.005060071811124278, 0.004274767378125488,
        -0.04336685815518482, -0.026057007392646048, 0.028942812209725187, 0.000529705961106153,
        0.1200696160803415, -0.0002931779871964968, 0.00007246839191387262, -0.029594076909184086,
        -0.006897438228015806, -0.03528844363185363, 0.07964540379157604, 0.05850918787168332,
        -0.003950776762991834, 0.060652859659549786, 0.0009524337266731062, 0.06605734818919765,
        0.0016335296923272007, -0.0007479677005420228, 0.002338960498723162, 0.02770841790278937,
        0.002981599159203041, -0.027353651039726756, 0.0023623963467092057, -0.002889919771864591
    };

    std::array<float, 4> coeff_b = {
        0.30749888793587216,
        0.10803677088312018,
        0.9403953878058084,
        -0.07136553129378706
    };
    // clang-format on

    return std::make_tuple(control_points, coeff_a, coeff_b);
}

} // namespace

TEST_CASE("primitives", "[implicit_shader]")
{
    SECTION("sphere")
    {
        implicit_shader::Sphere fn({0, 0, 0}, 1);

        std::array<float, 4> buffer;

        buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);
        REQUIRE_THAT(buffer[0], Catch::Matchers::WithinAbs(1, 1e-6));
        REQUIRE_THAT(buffer[1], Catch::Matchers::WithinAbs(0, 1e-6));
        REQUIRE_THAT(buffer[2], Catch::Matchers::WithinAbs(0, 1e-6));
        REQUIRE_THAT(buffer[3], Catch::Matchers::WithinAbs(0, 1e-6));

        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("cylinder")
    {
        implicit_shader::Cylinder fn({0, 0, 0}, {1, 1, 1}, 1);
        finite_difference_check(fn, {1, 0, 0});
    }

    SECTION("plane")
    {
        implicit_shader::Plane fn({0, 0, 0}, {1, 1, 1});
        finite_difference_check(fn, {1, 0, 0});
    }

    SECTION("cone")
    {
        implicit_shader::Cone fn({0, 0, 0}, {1, 1, 1}, 0.5);
        finite_difference_check(fn, {1, 0, 0});
    }

    SECTION("torus")
    {
        implicit_shader::Torus fn({0, 0, 0}, {1, 1, 1}, 1.0, 0.5);
        finite_difference_check(fn, {1, 0, 0});
    }

    SECTION("capsule")
    {
        implicit_shader::Capsule fn({0, 0, 0}, {1, 1, 1}, 1.0);
        finite_difference_check(fn, {1, 0, 0});
    }
}

TEST_CASE("duchon", "[implicit_shader][duchon]")
{
    SECTION("value check 1")
    {
        std::array<float, 3> control_points = {0, 0, 0};
        std::array<float, 4> coeff_a = {1, 0, 0, 0};
        std::array<float, 4> coeff_b = {0, 0, 0, 0};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);

        std::array<float, 4> buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);

        REQUIRE(buffer[3] == 1);
        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("value check 2")
    {
        std::array<float, 3> control_points = {0, 0, 0};
        std::array<float, 4> coeff_a = {0, 0, 0, 0};
        std::array<float, 4> coeff_b = {2, 0, 0, 0};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);

        std::array<float, 4> buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);

        REQUIRE(buffer[3] == 2);
        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("value check 3")
    {
        std::array<float, 3> control_points = {0, 0, 0};
        std::array<float, 4> coeff_a = {0, 1, 1, 1};
        std::array<float, 4> coeff_b = {0, 0, 0, 0};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);

        std::array<float, 4> buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);

        REQUIRE(buffer[3] == 3);
        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("value check 4")
    {
        std::array<float, 3> control_points = {0, 0, 0};
        std::array<float, 4> coeff_a = {0, 0, 0, 0};
        std::array<float, 4> coeff_b = {0, 1, 1, 1};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);

        std::array<float, 4> buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);

        REQUIRE(buffer[3] == 1);
        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("value check 5")
    {
        std::array<float, 6> control_points = {0, 0, 0, 2, 0, 0};
        std::array<float, 8> coeff_a = {1, 1, 0, 0, 0, 0, 0, 0};
        std::array<float, 4> coeff_b = {0, 0, 0, 0};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);

        std::array<float, 4> buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);

        REQUIRE(buffer[3] == 2);
        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("value check 6")
    {
        std::array<float, 6> control_points = {0, 0, 0, 2, 0, 0};
        std::array<float, 8> coeff_a = {0, 0, 1, 1, 1, 1, 1, 1};
        std::array<float, 4> coeff_b = {0, 0, 0, 0};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);

        std::array<float, 4> buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);

        REQUIRE(buffer[3] == 0);
        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("value check 7")
    {
        std::array<float, 6> control_points = {0, 0, 0, 2, 0, 0};
        std::array<float, 8> coeff_a = {0, 0, 0, 0, 0, 0, 0, 0};
        std::array<float, 4> coeff_b = {0, 1, 1, 1};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);

        std::array<float, 4> buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);

        REQUIRE(buffer[3] == 1);
        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("value check 8")
    {
        std::array<float, 6> control_points = {0, 0, 0, 2, 0, 0};
        std::array<float, 8> coeff_a = {0, 0, 0, 0, 0, 0, 0, 0};
        std::array<float, 4> coeff_b = {9, 0, 0, 0};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);

        std::array<float, 4> buffer = {1, 0, 0, 0};
        fn.evaluate(buffer);

        REQUIRE(buffer[3] == 9);
        finite_difference_check(fn, {10, 10, 10});
    }

    SECTION("value check 9")
    {
        std::array<float, 6> control_points = {1, 2, 3, 4, 5, 6};
        std::array<float, 8> coeff_a = {7, 8, 9, 10, 11, 12, 13, 14};
        std::array<float, 4> coeff_b = {15, 16, 17, 18};

        using namespace implicit_shader;
        Duchon fn(control_points, coeff_a, coeff_b);
        finite_difference_check(fn, {1, 1, 1});
    }

    SECTION("complex")
    {
        auto [control_points, coeff_a, coeff_b] = vipss_data();

        implicit_shader::Duchon fn(control_points, coeff_a, coeff_b);
        finite_difference_check(fn, {0.5, 0.5, 0.5}, 2e-3, 5e-2);
        finite_difference_check(fn, {0.25, 0.25, 0.25}, 2e-3, 5e-2);
        finite_difference_check(fn, {0.75, 0.25, 0.25}, 2e-3, 5e-2);
        finite_difference_check(fn, {0.75, 0.75, 0.25}, 2e-3, 5e-2);
        finite_difference_check(fn, {0.25, 0.75, 0.25}, 2e-3, 5e-2);
        finite_difference_check(fn, {0.75, 0.25, 0.75}, 2e-3, 5e-2);
        finite_difference_check(fn, {0.25, 0.75, 0.75}, 2e-3, 5e-2);
        finite_difference_check(fn, {0.75, 0.75, 0.75}, 2e-3, 5e-2);

        // Compare with implicit functions implementation
        std::vector<Hermite_RBF<float>::Vec3> control_points_copy(control_points.size() / 3);
        for (size_t i = 0; i < control_points_copy.size(); ++i) {
            control_points_copy[i] = {
                control_points[i * 3],
                control_points[i * 3 + 1],
                control_points[i * 3 + 2]};
        }
        Hermite_RBF<float>::VecX coeef_a_copy(coeff_a.size());
        std::copy(coeff_a.begin(), coeff_a.end(), coeef_a_copy.data());
        Hermite_RBF<float>::Vec4 coeff_b_copy;
        std::copy(coeff_b.begin(), coeff_b.end(), coeff_b_copy.data());
        Hermite_RBF<float> fn2(control_points_copy, coeef_a_copy, coeff_b_copy);

        for (float x = 0; x < 1; x += 0.1) {
            for (float y = 0; y < 1; y += 0.1) {
                for (float z = 0; z < 1; z += 0.1) {
                    std::array<float, 4> buffer = {x, y, z, 0};
                    {
                        auto r = fn2.evaluate(buffer[0], buffer[1], buffer[2]);
                        float gx, gy, gz;
                        fn2.evaluate_gradient(buffer[0], buffer[1], buffer[2], gx, gy, gz);
                        fn.evaluate(buffer);
                        REQUIRE_THAT(buffer[3], Catch::Matchers::WithinAbs(r, 1e-3));
                        REQUIRE_THAT(buffer[0], Catch::Matchers::WithinAbs(gx, 1e-3));
                        REQUIRE_THAT(buffer[1], Catch::Matchers::WithinAbs(gy, 1e-3));
                        REQUIRE_THAT(buffer[2], Catch::Matchers::WithinAbs(gz, 1e-3));
                    }
                }
            }
        }
    }
}

TEST_CASE("benchmark", "[implicit_shader][duchon][.benchmark]")
{
    auto [control_points, coeff_a, coeff_b] = vipss_data();
    implicit_shader::Duchon fn(control_points, coeff_a, coeff_b);

    std::vector<Hermite_RBF<float>::Vec3> control_points_copy(control_points.size() / 3);
    for (size_t i = 0; i < control_points_copy.size(); ++i) {
        control_points_copy[i] = {
            control_points[i * 3],
            control_points[i * 3 + 1],
            control_points[i * 3 + 2]};
    }
    Hermite_RBF<float>::VecX coeef_a_copy(coeff_a.size());
    std::copy(coeff_a.begin(), coeff_a.end(), coeef_a_copy.data());
    Hermite_RBF<float>::Vec4 coeff_b_copy;
    std::copy(coeff_b.begin(), coeff_b.end(), coeff_b_copy.data());
    Hermite_RBF<float> fn2(control_points_copy, coeef_a_copy, coeff_b_copy);

    constexpr size_t N = 100;
    std::vector<float> buffer(4 * N * N * N);
    for (int i = 0; i < N * N * N; ++i) {
        buffer[i * 4] = float(i % N) / float(N);
        buffer[i * 4 + 1] = float((i / N) % N) / float(N);
        buffer[i * 4 + 2] = float(i / N / N) / float(N);
    }
    BENCHMARK("Shader")
    {
        return fn.evaluate(buffer);
    };
    BENCHMARK("function")
    {
        float value = 0;
        float gx, gy, gz;
        for (int i = 0; i < N * N * N; ++i) {
            value += fn2.evaluate(buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2]);
            fn2.evaluate_gradient(buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2], gx, gy, gz);
            value += gx + gy + gz;
        }
        return value;
    };
}
