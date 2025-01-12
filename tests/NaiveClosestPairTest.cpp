// NaiveClosestPairPointTest.cpp

#include "gtest/gtest.h"
#include "straight_skeleton/event_management/NaiveClosestPair.h"

#include <memory>
#include <stdexcept>

#include <unordered_set>

namespace straight_skeleton {

    double dist_func(const Point& a, const Point& b) {
        Vector dist_vector = a - b;
        return sqrt(CGAL::to_double(dist_vector.squared_length()));
    }

    using NaiveClosestPairPoint = NaiveClosestPair<Point, Point, double(*)(const Point&, const Point&)>;
    using NaiveClosestPairPtr = std::shared_ptr<NaiveClosestPairPoint>;

// Annahme: PointWrapper ist bereits definiert mit operator== und std::hash-Spezialisierung.

class NaiveClosestPairPointTest : public ::testing::Test {
protected:
    // Instanz der NaiveClosestPair-Klasse
    NaiveClosestPairPtr closestPair;

    // Testobjekte
    Point point1;
    Point point2;
    Point point3;

    Point pointA;
    Point pointB;
    Point pointC;

    void SetUp() override {
        // Initialisiere die NaiveClosestPair-Instanz mit der Distanzfunktion
        closestPair = std::make_shared<NaiveClosestPairPoint>(dist_func);

        // Initialisiere Punkte für Point
        point1 = Point(0, 0);
        point2 = Point(1, 1);
        point3 = Point(2, 2);

        // Initialisiere Punkte für Point
        pointA = Point(0.2, 0.2);
        pointB = Point(1.5, 1.5);
        pointC = Point(3, 3);
    }
};

// Test des Einfügens von R und B Objekten
TEST_F(NaiveClosestPairPointTest, InsertElements) {
    // Füge R- und B-Punkte hinzu
    EXPECT_NO_THROW(closestPair->insertR(point1));
    EXPECT_NO_THROW(closestPair->insertR(point2));
    EXPECT_NO_THROW(closestPair->insertB(pointA));
    EXPECT_NO_THROW(closestPair->insertB(pointB));

    // Teste doppelte Einfügungen
    EXPECT_THROW(closestPair->insertR(point1), std::invalid_argument);
    EXPECT_THROW(closestPair->insertB(pointA), std::invalid_argument);
}

// Test des Entfernens von R und B Objekten
TEST_F(NaiveClosestPairPointTest, RemoveElements) {
    // Füge R- und B-Punkte hinzu
    closestPair->insertR(point1);
    closestPair->insertR(point2);
    closestPair->insertB(pointA);
    closestPair->insertB(pointB);

    // Entferne einen R-Punkt und einen B-Punkt
    EXPECT_NO_THROW(closestPair->removeR(point1));
    EXPECT_NO_THROW(closestPair->removeB(pointA));

    // Versuche, nicht vorhandene Punkte zu entfernen
    EXPECT_THROW(closestPair->removeR(point3), std::invalid_argument);
    EXPECT_THROW(closestPair->removeB(pointC), std::invalid_argument);
}

// Test des minimalen Paares
TEST_F(NaiveClosestPairPointTest, GetMinPair) {
    // Füge R- und B-Punkte hinzu
    closestPair->insertR(point1);
    closestPair->insertR(point2);
    closestPair->insertB(pointA);
    closestPair->insertB(pointB);
    closestPair->insertB(pointC);

    // Berechne erwartetes minimales Paar (point1 und pointA)
    auto minPair = closestPair->getMinPair();
    ASSERT_TRUE(minPair.has_value());
    EXPECT_EQ(minPair->first, point1);
    EXPECT_EQ(minPair->second, pointA);
}

// Test des Extract-Min-Paares
TEST_F(NaiveClosestPairPointTest, ExtractMinPair) {
    // Füge R- und B-Punkte hinzu
    closestPair->insertR(point1);
    closestPair->insertR(point2);
    closestPair->insertB(pointA);
    closestPair->insertB(pointB);
    closestPair->insertB(pointC);

    // Extrahiere das minimale Paar (point1 und pointA)
    auto minPair = closestPair->extractMinPair();
    ASSERT_TRUE(minPair.has_value());
    EXPECT_EQ(minPair->first, point1);
    EXPECT_EQ(minPair->second, pointA);

    // Das minimale Paar sollte nun entfernt sein, das nächste minimale Paar ist (point2 und pointB)
    auto newMinPair = closestPair->getMinPair();
    ASSERT_TRUE(newMinPair.has_value());
    EXPECT_EQ(newMinPair->first, point2);
    EXPECT_EQ(newMinPair->second, pointB);
}

// Test, ob die Datenstruktur leer ist
TEST_F(NaiveClosestPairPointTest, EmptyCheck) {
    EXPECT_TRUE(closestPair->empty());

    closestPair->insertR(point1);
    EXPECT_TRUE(closestPair->empty()); // B ist noch leer

    closestPair->insertB(pointA);
    EXPECT_FALSE(closestPair->empty());

    closestPair->removeR(point1);
    EXPECT_TRUE(closestPair->empty());

    closestPair->removeB(pointA);
    EXPECT_TRUE(closestPair->empty());
}

// Test der Heap-Rebuild-Funktionalität
TEST_F(NaiveClosestPairPointTest, RebuildHeap) {
    // Füge mehrere R- und B-Punkte hinzu
    closestPair->insertR(point1);
    closestPair->insertR(point2);
    closestPair->insertR(point3);
    closestPair->insertB(pointA);
    closestPair->insertB(pointB);
    closestPair->insertB(pointC);

    // Extrahiere das minimale Paar (point1 und pointA)
    auto minPair1 = closestPair->extractMinPair();
    ASSERT_TRUE(minPair1.has_value());
    EXPECT_EQ(minPair1->first, point1);
    EXPECT_EQ(minPair1->second, pointA);

    // Entferne point2 und pointB, was den Heap neu aufbauen sollte
    EXPECT_NO_THROW(closestPair->removeR(point2));
    EXPECT_NO_THROW(closestPair->removeB(pointB));
    EXPECT_THROW(closestPair->removeB(pointA), std::invalid_argument);

    // Das nächste minimale Paar sollte (point3 und pointC) sein
    auto minPair2 = closestPair->getMinPair();
    ASSERT_TRUE(minPair2.has_value());
    EXPECT_EQ(minPair2->first, point3);
    EXPECT_EQ(minPair2->second, pointC);
}

// Test der expliziten Template-Instanziierung
TEST_F(NaiveClosestPairPointTest, TemplateInstantiation) {
    std::cout << closestPair->rSetSize() << std::endl;
    std::cout << closestPair->bSetSize() << std::endl;

    // Füge R- und B-Punkte hinzu
    closestPair->insertR(point1);
    closestPair->insertB(pointA);

    std::cout << closestPair->rSetSize() << std::endl;
    std::cout << closestPair->bSetSize() << std::endl;

    // Extrahiere das minimale Paar
    auto minPair = closestPair->extractMinPair();
    ASSERT_TRUE(minPair.has_value());
    EXPECT_EQ(minPair->first, point1);
    EXPECT_EQ(minPair->second, pointA);

    std::cout << closestPair->rSetSize() << std::endl;
    std::cout << closestPair->bSetSize() << std::endl;

    // Nach dem Extrahieren sollte die Datenstruktur leer sein
    EXPECT_TRUE(closestPair->empty());
}

} // namespace straight_skeleton
