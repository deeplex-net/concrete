
// Copyright Henrik Steffen Gaßmann 2019
//
// Distributed under the Boost Software License, Version 1.0.
//         (See accompanying file LICENSE or copy at
//           https://www.boost.org/LICENSE_1_0.txt)

#include "dplx/cncr/data_defined_status_domain.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <status-code/nested_status_code.hpp>
#include <status-code/quick_status_code_from_enum.hpp>
#include <status-code/system_code.hpp>

#include "test_utils.hpp"

namespace cncr_tests
{
enum class test_errc
{
    success = 0,
    perm = 3,
    other_success = 5,
    not_implemented = 7,
};
// using dplx::cncr::make_status_code;

namespace
{

} // namespace
} // namespace cncr_tests

template <>
struct dplx::cncr::status_enum_definition<cncr_tests::test_errc>
    : status_enum_definition_defaults<cncr_tests::test_errc>
{
    static constexpr char domain_id[]
            = "{09E0ECBF-A737-454D-8633-17E733CDE15F}";
    static constexpr char domain_name[] = "test-domain";

    static constexpr value_descriptor values[] = {
            {        code::success,           generic_errc::success,       "yay!"},
            {           code::perm, generic_errc::permission_denied,      "oh no"},
            {  code::other_success,           generic_errc::success,     "oh kay"},
            {code::not_implemented,           generic_errc::unknown, "till later"},
    };
};

namespace cncr_tests
{

using test_domain = dplx::cncr::data_defined_status_domain_type<test_errc>;
using test_code = dplx::cncr::data_defined_status_code<test_errc>;

TEST_CASE("test_errc can be converted to status_code")
{
    test_code c = test_errc::success;
    CHECK(c.value() == test_errc::success);
}

TEST_CASE("data defined domain forwards the domain uuid")
{
    constexpr auto parsed = cncr::system_error::detail::parse_uuid_from_array(
            cncr::status_enum_definition<test_errc>::domain_id);

    CHECK(test_code::domain_type::get().id() == parsed);
}

TEST_CASE("data defined domain forwards the domain name")
{
    CHECK(std::string_view(test_code::domain_type::get().name().data())
          == std::string_view(
                  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
                  cncr::status_enum_definition<test_errc>::domain_name));
}

TEST_CASE("data defined domain translates failure and success")
{
    CHECK(test_code(test_errc::success).success());
    CHECK(test_code(test_errc::other_success).success());

    CHECK(test_code(test_errc::perm).failure());
    CHECK(test_code(test_errc::not_implemented).failure());
}

TEST_CASE("data defined domain allows generic comparisons")
{
    using errc = cncr::system_error::errc;

    auto &&[code, equivalent] = GENERATE(table<test_errc, errc>({
            {        test_errc::success,           errc::success},
            {           test_errc::perm, errc::permission_denied},
            {  test_errc::other_success,           errc::success},
            {test_errc::not_implemented,           errc::unknown},
    }));

    CHECK(test_code(code) == equivalent);
    CHECK(equivalent == test_code(code));
}

TEST_CASE("data defined domain returns the correct description")
{
    using enum test_errc;
    using namespace std::string_view_literals;

    auto &&[code, description] = GENERATE(table<test_errc, std::string_view>({
            {        success,       "yay!"sv},
            {           perm,      "oh no"sv},
            {  other_success,     "oh kay"sv},
            {not_implemented, "till later"sv},
    }));

    CHECK(std::string_view(test_code(code).message().data()) == description);
}

TEST_CASE("data defined domain can be put on the heap")
{
    cncr::system_error::system_code const subject
            = cncr::system_error::make_nested_status_code(
                    test_code{test_errc::perm});
    CHECK(subject == test_errc::perm);
}

} // namespace cncr_tests
