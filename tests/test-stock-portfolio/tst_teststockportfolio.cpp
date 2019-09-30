#include <QCoreApplication>
#include <QDate>
#include <QString>
#include <QtTest>
#include <portfolio.h>
#include <stdexcept>

#define Given(func) func
#define And(func) func
#define When(func) func
#define Then(func) func
#define NOT_IMPLEMENTED Q_ASSERT_X(false, Q_FUNC_INFO, "Not Implemented");

class StubRepository : public IRepository {
public:
    QVector<Transaction> all() override
    {
        return m_transactions;
    }

    void buyShare(QString name, int quantity, QDate date)
    {
        m_transactions.append({ name, quantity, date });
    }

    void sellShare(QString name, int quantity, QDate date)
    {
        m_transactions.append({ name, -quantity, date });
    }

    void setValue(QString name, double value)
    {
        m_values[name] = value;
    }

    double currentValue(QString company)
    {
        return m_values.value(company);
    }

private:
    QVector<Transaction> m_transactions;
    QHash<QString, double> m_values;
};

class TestStockPortfolio : public QObject {
    Q_OBJECT
public:
private slots:
    void init();
    void cleanup(){};
    void testPrintPortfolio();

private:
    Portfolio* m_portfolio{ nullptr };
    StubRepository* m_repo{ nullptr };
    QString m_output;

    void IBoughtShares(QString company, int quantity, QDate date)
    {
        m_repo->buyShare(company, quantity, date);
    }
    void ISoldShares(QString company, int quantity, QDate date)
    {
        m_repo->sellShare(company, quantity, date);
    }
    void CurrentShareValueOf(QString company, double value)
    {
        m_repo->setValue(company, value);
    }
    void IPrintPortfolio()
    {
        m_output = m_portfolio->currentState();
    }
};

void TestStockPortfolio::init()
{
    m_repo = new StubRepository;
    m_portfolio = new Portfolio(m_repo);
}

void TestStockPortfolio::testPrintPortfolio()
{
    const QString oswsLTDName = "Old School Waterfall Software LTD";
    const QString cmLTDName = "Crafter Master Limited";
    const QString xppIncName = "XP Practitioners Incorporated";
    const QString expectedPrint = "company | shares | current price | current value | last operation"
                                  "Old School Waterfall Software LTD | 500 | $5.75 | $2,875.00 | sold 500 on 11/12/2018"
                                  "Crafter Masters Limited | 400 | $17.25 | $6,900.00 | bought 400 on 09/06/2016"
                                  "XP Practitioners Incorporated | 700 | $25.55 | $17,885.00 | bought 700 on 10/12/2018";

    Given(IBoughtShares(oswsLTDName, 1000, QDate(1990, 2, 14)));
    And(IBoughtShares(cmLTDName, 400, QDate(2016, 6, 9)));
    And(IBoughtShares(xppIncName, 700, QDate(2018, 12, 10)));
    And(ISoldShares(oswsLTDName, 500, QDate(2018, 12, 11)));
    And(CurrentShareValueOf(oswsLTDName, 5.75));
    And(CurrentShareValueOf(cmLTDName, 17.25));
    And(CurrentShareValueOf(xppIncName, 25.55));

    When(IPrintPortfolio());

    Then(QCOMPARE(expectedPrint, m_output));
}

QTEST_MAIN(TestStockPortfolio)

#include "tst_teststockportfolio.moc"
