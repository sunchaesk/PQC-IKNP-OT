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

///////////////////////////////
// Polynomial Vector Tests
///////////////////////////////

TEST_F(PolynomialTest, PolynomialVectorConstructorTest) {
    Polynomial p1({1, 2, 3}, 17);
    Polynomial p2({4, 5, 6}, 17);
    Polynomial p3({7, 8, 9}, 17);

    std::vector<Polynomial> polys = {p1, p2, p3};
    PolynomialVector vec(polys);

    ASSERT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.get_is_vertical(), true);
    EXPECT_EQ(vec.get_polynomials().size(), 3);
}

TEST_F(PolynomialTest, PolynomialVectorAdditionTest) {
    Polynomial p1({1, 2, 3}, 17);
    Polynomial p2({4, 5, 6}, 17);
    Polynomial p3({7, 8, 9}, 17);

    Polynomial q1({2, 3, 4}, 17);
    Polynomial q2({5, 6, 7}, 17);
    Polynomial q3({8, 9, 10}, 17);

    PolynomialVector vec1({p1, p2, p3});
    PolynomialVector vec2({q1, q2, q3});

    PolynomialVector result = vec1 + vec2;

    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result.get_polynomials()[0].coeffs(), (std::vector<uint64_t>{3, 5, 7}));
    EXPECT_EQ(result.get_polynomials()[1].coeffs(), (std::vector<uint64_t>{9, 11, 13}));
    EXPECT_EQ(result.get_polynomials()[2].coeffs(), (std::vector<uint64_t>{15, 0, 2}));
}

TEST_F(PolynomialTest, PolynomialVectorSubtractionTest) {
    Polynomial p1({10, 11, 12}, 17);
    Polynomial p2({14, 15, 16}, 17);

    Polynomial q1({3, 4, 5}, 17);
    Polynomial q2({6, 7, 8}, 17);

    PolynomialVector vec1({p1, p2});
    PolynomialVector vec2({q1, q2});

    PolynomialVector result = vec1 - vec2;

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result.get_polynomials()[0].coeffs(), (std::vector<uint64_t>{7, 7, 7}));
    EXPECT_EQ(result.get_polynomials()[1].coeffs(), (std::vector<uint64_t>{8, 8, 8}));
}

TEST_F(PolynomialTest, PolynomialVectorInnerProductTest) {
    // Simple inner product test
    Polynomial p1({1, 0, 0}, 17);
    Polynomial p2({2, 0, 0}, 17);

    Polynomial q1({3, 0, 0}, 17);
    Polynomial q2({4, 0, 0}, 17);

    PolynomialVector vec1({p1, p2});
    PolynomialVector vec2({q1, q2});

    Polynomial result = vec1.inner_product(vec2);

    // p1 * q1 + p2 * q2 = (1)(3) + (2)(4) = 3 + 8 = 11
    EXPECT_EQ(result.coeffs(), (std::vector<uint64_t>{11, 0, 0}));

    // More complex test with actual polynomial multiplication
    Polynomial a1({1, 2}, 13);
    Polynomial a2({3, 4}, 13);

    Polynomial b1({5, 6}, 13);
    Polynomial b2({7, 8}, 13);

    PolynomialVector vecA({a1, a2});
    PolynomialVector vecB({b1, b2});

    Polynomial result2 = vecA.inner_product(vecB);

    // a1 * b1 = (1 + 2x)(5 + 6x) = 5 + 6x + 10x + 12x^2 = 5 + 16x - 12 = -7 + 16x = 6 + 3x (mod 13)
    // a2 * b2 = (3 + 4x)(7 + 8x) = 21 + 24x + 28x + 32x^2 = 21 + 52x - 32 = -11 + 52x = 2 + 0x (mod 13)
    // sum = 6 + 3x + 2 + 0x = 8 + 3x
    EXPECT_EQ(result2.coeffs(), (std::vector<uint64_t>{8, 3}));
}

TEST_F(PolynomialTest, PolynomialVectorMismatchedSizeTest) {
    Polynomial p1({1, 2, 3}, 17);
    Polynomial p2({4, 5, 6}, 17);
    Polynomial p3({7, 8, 9}, 17);

    PolynomialVector vec1({p1, p2});
    PolynomialVector vec2({p1, p2, p3});

    EXPECT_THROW(vec1 + vec2, std::invalid_argument);
    EXPECT_THROW(vec1 - vec2, std::invalid_argument);
    EXPECT_THROW(vec1.inner_product(vec2), std::invalid_argument);
}

///////////////////////////////
// Polynomial Matrix Tests
///////////////////////////////

TEST_F(PolynomialTest, PolynomialMatrixConstructorTest) {
    Polynomial p1({1, 2}, 17);
    Polynomial p2({3, 4}, 17);
    Polynomial p3({5, 6}, 17);
    Polynomial p4({7, 8}, 17);

    std::vector<std::vector<Polynomial>> mat = {{p1, p2}, {p3, p4}};
    PolynomialMatrix matrix(mat);

    EXPECT_EQ(matrix.get_rows(), 2);
    EXPECT_EQ(matrix.get_cols(), 2);
}

TEST_F(PolynomialTest, PolynomialMatrixTransposeTest) {
    // Test 2x2 matrix transpose
    Polynomial p1({1, 0}, 17);
    Polynomial p2({2, 0}, 17);
    Polynomial p3({3, 0}, 17);
    Polynomial p4({4, 0}, 17);

    std::vector<std::vector<Polynomial>> mat = {{p1, p2}, {p3, p4}};
    PolynomialMatrix matrix(mat);

    PolynomialMatrix transposed = matrix.transpose();

    EXPECT_EQ(transposed.get_rows(), 2);
    EXPECT_EQ(transposed.get_cols(), 2);

    // Original matrix: [p1 p2]  Transposed: [p1 p3]
    //                  [p3 p4]              [p2 p4]

    EXPECT_EQ(transposed.get_matrix()[0][0].coeffs(), p1.coeffs());
    EXPECT_EQ(transposed.get_matrix()[0][1].coeffs(), p3.coeffs());
    EXPECT_EQ(transposed.get_matrix()[1][0].coeffs(), p2.coeffs());
    EXPECT_EQ(transposed.get_matrix()[1][1].coeffs(), p4.coeffs());
}

TEST_F(PolynomialTest, PolynomialMatrixTransposeNonSquareTest) {
    // Test 2x3 matrix transpose
    Polynomial p1({1, 0}, 17);
    Polynomial p2({2, 0}, 17);
    Polynomial p3({3, 0}, 17);
    Polynomial p4({4, 0}, 17);
    Polynomial p5({5, 0}, 17);
    Polynomial p6({6, 0}, 17);

    // Matrix: [p1 p2 p3]
    //         [p4 p5 p6]
    std::vector<std::vector<Polynomial>> mat = {{p1, p2, p3}, {p4, p5, p6}};
    PolynomialMatrix matrix(mat);

    EXPECT_EQ(matrix.get_rows(), 2);
    EXPECT_EQ(matrix.get_cols(), 3);

    PolynomialMatrix transposed = matrix.transpose();

    EXPECT_EQ(transposed.get_rows(), 3);
    EXPECT_EQ(transposed.get_cols(), 2);

    // Transposed: [p1 p4]
    //             [p2 p5]
    //             [p3 p6]

    EXPECT_EQ(transposed.get_matrix()[0][0].coeffs(), p1.coeffs());
    EXPECT_EQ(transposed.get_matrix()[0][1].coeffs(), p4.coeffs());
    EXPECT_EQ(transposed.get_matrix()[1][0].coeffs(), p2.coeffs());
    EXPECT_EQ(transposed.get_matrix()[1][1].coeffs(), p5.coeffs());
    EXPECT_EQ(transposed.get_matrix()[2][0].coeffs(), p3.coeffs());
    EXPECT_EQ(transposed.get_matrix()[2][1].coeffs(), p6.coeffs());
}

TEST_F(PolynomialTest, PolynomialMatrixDoubleTransposeTest) {
    // Test that (A^T)^T = A
    Polynomial p1({1, 2}, 17);
    Polynomial p2({3, 4}, 17);
    Polynomial p3({5, 6}, 17);
    Polynomial p4({7, 8}, 17);
    Polynomial p5({9, 10}, 17);
    Polynomial p6({11, 12}, 17);

    std::vector<std::vector<Polynomial>> mat = {{p1, p2, p3}, {p4, p5, p6}};
    PolynomialMatrix matrix(mat);

    PolynomialMatrix double_transposed = matrix.transpose().transpose();

    EXPECT_EQ(double_transposed.get_rows(), matrix.get_rows());
    EXPECT_EQ(double_transposed.get_cols(), matrix.get_cols());

    for (size_t i = 0; i < matrix.get_rows(); i++) {
        for (size_t j = 0; j < matrix.get_cols(); j++) {
            EXPECT_EQ(double_transposed.get_matrix()[i][j].coeffs(),
                     matrix.get_matrix()[i][j].coeffs());
        }
    }
}

TEST_F(PolynomialTest, PolynomialMatrixVectorMultiplicationTest) {
    // Test 2x2 matrix times 2x1 vector
    Polynomial p1({1, 0}, 17);
    Polynomial p2({2, 0}, 17);
    Polynomial p3({3, 0}, 17);
    Polynomial p4({4, 0}, 17);

    // Matrix: [p1 p2]  = [1 2]
    //         [p3 p4]    [3 4]
    std::vector<std::vector<Polynomial>> mat = {{p1, p2}, {p3, p4}};
    PolynomialMatrix matrix(mat);

    Polynomial v1({5, 0}, 17);
    Polynomial v2({6, 0}, 17);

    // Vector: [v1] = [5]
    //         [v2]   [6]
    PolynomialVector vec({v1, v2}, true);

    PolynomialVector result = matrix.matrix_vector_mul(vec);

    // Result: [1*5 + 2*6] = [17] = [0]  (mod 17)
    //         [3*5 + 4*6]   [39]   [5]

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result.get_polynomials()[0].coeffs(), (std::vector<uint64_t>{0, 0}));
    EXPECT_EQ(result.get_polynomials()[1].coeffs(), (std::vector<uint64_t>{5, 0}));
}

TEST_F(PolynomialTest, PolynomialMatrixVectorMultiplicationComplexTest) {
    // Test with actual polynomial multiplication
    Polynomial p1({1, 1}, 13);  // 1 + x
    Polynomial p2({2, 1}, 13);  // 2 + x
    Polynomial p3({3, 1}, 13);  // 3 + x
    Polynomial p4({4, 1}, 13);  // 4 + x

    std::vector<std::vector<Polynomial>> mat = {{p1, p2}, {p3, p4}};
    PolynomialMatrix matrix(mat);

    Polynomial v1({1, 0}, 13);  // 1
    Polynomial v2({0, 1}, 13);  // x

    PolynomialVector vec({v1, v2}, true);

    PolynomialVector result = matrix.matrix_vector_mul(vec);

    // Row 1: (1+x)*1 + (2+x)*x = 1+x + 2x+x^2 = 1+3x-1 = 3x
    // Row 2: (3+x)*1 + (4+x)*x = 3+x + 4x+x^2 = 3+5x-1 = 2+5x

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result.get_polynomials()[0].coeffs(), (std::vector<uint64_t>{0, 3}));
    EXPECT_EQ(result.get_polynomials()[1].coeffs(), (std::vector<uint64_t>{2, 5}));
}

TEST_F(PolynomialTest, PolynomialMatrixVectorMismatchedDimensionsTest) {
    Polynomial p1({1, 0}, 17);
    Polynomial p2({2, 0}, 17);
    Polynomial p3({3, 0}, 17);
    Polynomial p4({4, 0}, 17);

    std::vector<std::vector<Polynomial>> mat = {{p1, p2}, {p3, p4}};
    PolynomialMatrix matrix(mat);

    Polynomial v1({5, 0}, 17);

    // Vector with size 1, but matrix has 2 columns
    PolynomialVector vec({v1}, true);

    EXPECT_THROW(matrix.matrix_vector_mul(vec), std::invalid_argument);
}

TEST_F(PolynomialTest, PolynomialMatrixVectorNonVerticalTest) {
    Polynomial p1({1, 0}, 17);
    Polynomial p2({2, 0}, 17);
    Polynomial p3({3, 0}, 17);
    Polynomial p4({4, 0}, 17);

    std::vector<std::vector<Polynomial>> mat = {{p1, p2}, {p3, p4}};
    PolynomialMatrix matrix(mat);

    Polynomial v1({5, 0}, 17);
    Polynomial v2({6, 0}, 17);

    // Vector marked as horizontal (not vertical)
    PolynomialVector vec({v1, v2}, false);

    EXPECT_THROW(matrix.matrix_vector_mul(vec), std::invalid_argument);
}