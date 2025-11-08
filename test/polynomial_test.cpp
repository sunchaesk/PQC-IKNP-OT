#include <gtest/gtest.h>
#include "../include/polynomial.h"

class PolynomialTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(PolynomialTest, PolynomialConstructorTest) {
    std::vector<uint64_t> coeffs = {1, 2, 3, 4};
    Polynomial poly(coeffs, 17);
    ASSERT_EQ(poly.degree(), 4);
    ASSERT_EQ(poly.coeffs(), coeffs);
}

TEST_F(PolynomialTest, PolynomialAdditionTest) {
    // test simple addition
    Polynomial a({1, 2, 3}, 12);
    Polynomial b({4, 5, 6}, 12);

    Polynomial c = a + b;

    EXPECT_EQ(c.coeffs(), (std::vector<uint64_t>{5, 7, 9}));

    // test addition with modulus
    Polynomial d({10, 11, 12}, 12);
    Polynomial e({1, 2, 3}, 12);
    Polynomial f = d + e;

    EXPECT_EQ(f.coeffs(), (std::vector<uint64_t>{11, 1, 3}));

    // test addition zero
    Polynomial g({0, 0, 0}, 12);
    Polynomial h = a + g;
    EXPECT_EQ(h.coeffs(), a.coeffs());
}

TEST_F(PolynomialTest, MismatchedParametersTest) {
    // have only tested for addition so far
    Polynomial a({1, 2, 3}, 12);
    Polynomial b({4, 5, 6}, 13); // diff q
    Polynomial c({1, 1, 1, 1}, 12); // diff N

    EXPECT_THROW(a + b, std::invalid_argument);
    EXPECT_THROW(a + c, std::invalid_argument);
}

TEST_F(PolynomialTest, PolynomialMultiplicationPropertyTest) {
    // testing polynomial multiplication properties

    // commutativity
    Polynomial a({11, 2, 3, 9}, 137);
    Polynomial b({2, 65, 1, 1}, 137);
    Polynomial c = a * b;
    Polynomial d = b * a;
    EXPECT_EQ(c.coeffs(), d.coeffs());

    // associativity
    Polynomial e({1, 2, 3, 4}, 137);
    Polynomial f = a * (b * e);
    Polynomial g = (a * b) * e;
    EXPECT_EQ(f.coeffs(), g.coeffs());

    // distributivity
    Polynomial h = a * (b + e);
    Polynomial i = (a * b) + (a * e);
    EXPECT_EQ(h.coeffs(), i.coeffs());

    // multiplication identity
    Polynomial iden = a * Polynomial({1, 0, 0, 0}, 137);
    EXPECT_EQ(iden.coeffs(), a.coeffs());
    EXPECT_EQ(iden.coeffs(), iden.multiply_scalar(1).coeffs());

    // additivity zero
    Polynomial zero = a + Polynomial({0, 0, 0, 0}, 137);
    EXPECT_EQ(zero.coeffs(), a.coeffs());

    // multiplication by zero
    Polynomial zero_mul = a * Polynomial({0, 0, 0, 0}, 137);
    EXPECT_EQ(zero_mul.coeffs(), (std::vector<uint64_t>{0, 0, 0, 0}));
}

// keep adding tests -> multiplication correctness is crucial
TEST_F(PolynomialTest, PolynomialMultiplicationTest) {
    Polynomial a({1, 2, 3, 4}, 7681);
    Polynomial b({5, 6, 7, 8}, 7681);
    Polynomial c = a * b;
    EXPECT_EQ(c.coeffs(), (std::vector<uint64_t>{7625, 7645, 2, 60}));

    // the test cases below are generated manually using sagemath
    Polynomial d({1, 2, 3, 0}, 7681);
    Polynomial e({2, 0, 1, 1}, 7681);
    Polynomial f = d * e;
    EXPECT_EQ(f.coeffs(), (std::vector<uint64_t>{7678, 1, 7, 3}));

    Polynomial aa({1, 2, 3, 0}, 11);
    Polynomial bb({2, 0, 1, 1}, 11);
    Polynomial cc = aa * bb;
    EXPECT_EQ(cc.coeffs(), (std::vector<uint64_t>{8, 1, 7, 3}));
}

// negative scalar multiply should be handled correctly
// let n < 0
// let n' = ((n mod q) + q) mod q
// multiply each element by n'
// then modular reduce
TEST_F(PolynomialTest, NegativeScalarMultiplicationTest) {
    Polynomial a({1, 2, 3, 4}, 17);
    Polynomial b = a.multiply_scalar(-3);
    EXPECT_EQ(b.coeffs(), (std::vector<uint64_t>{14, 11, 8, 5}));

    Polynomial c({1, 2, 3, 4}, 19);
    Polynomial d = c.multiply_scalar(-3);
    EXPECT_EQ(d.coeffs(), (std::vector<uint64_t>{16, 13, 10, 7}));
}

TEST_F(PolynomialTest, ScalarMultiplicationTest) {
    Polynomial a({1, 2, 3, 4}, 11);
    Polynomial b = a.multiply_scalar(3);
    EXPECT_EQ(b.coeffs(), (std::vector<uint64_t>{3, 6, 9, 1}));

    Polynomial c = a.multiply_scalar(0);
    EXPECT_EQ(c.coeffs(), (std::vector<uint64_t>{0, 0, 0, 0}));

    Polynomial d = a.multiply_scalar(1);
    EXPECT_EQ(d.coeffs(), a.coeffs());

    Polynomial e = a.multiply_scalar(11);
    EXPECT_EQ(e.coeffs(), (std::vector<uint64_t>{0, 0, 0, 0}));

    // other tests
    Polynomial f({5, 6, 7, 8}, 13);
    Polynomial g = f.multiply_scalar(4);
    EXPECT_EQ(g.coeffs(), (std::vector<uint64_t>{7, 11, 2, 6}));
}

TEST_F(PolynomialTest, HigherNPolynomialTest) {
    Polynomial a({1, 2, 3, 4, 5, 6, 7, 8}, 7681);
    Polynomial b({8, 7, 6, 5, 4, 3, 2, 1}, 7681);
    Polynomial c = a * b;
    EXPECT_EQ(c.coeffs(), (std::vector<uint64_t>{7521, 7571, 7625, 0, 56, 110, 160, 204}));

    Polynomial d({1, 0, 2, 0, 3, 0, 4, 0}, 224);
    Polynomial e({0, 5, 0, 6, 0, 7, 0, 8}, 224);
    Polynomial f = d * e;
    EXPECT_EQ(f.coeffs(), (std::vector<uint64_t>{0, 168, 0, 188, 0, 2, 0, 60}));
}