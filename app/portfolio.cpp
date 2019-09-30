#include "portfolio.h"

Portfolio::Portfolio(IRepository* repo)
    : m_repo(repo)
{
}

QString Portfolio::currentState()
{
    auto transactions = m_repo->all();

    struct ReportLine {
        QString company;
        double totalShareValue;
        Transaction lastTransaction;
        Transaction firstTransaction;
    };

    QHash<QString, ReportLine> reportLines;

    for (auto& t : qAsConst(transactions)) {
        double valueShare = m_repo->currentValue(t.company);

        if (!reportLines.contains(t.company)) {
            ReportLine line;
            line.company = t.company;
            line.firstTransaction = t;
            line.lastTransaction = t;
            line.totalShareValue = t.quantity * valueShare;
        } else {
            auto& line = reportLines[t.company];
            line.lastTransaction = t;
            line.totalShareValue = line.totalShareValue + t.quantity * valueShare;
        }
    }

    auto report = reportLines.values().toVector();
    std::sort(report.begin(), report.end(), [&](ReportLine l1, ReportLine l2) {
        l1.firstTransaction.date < l2.firstTransaction.date;
    });

    QString reportString = reportHeader;
}
