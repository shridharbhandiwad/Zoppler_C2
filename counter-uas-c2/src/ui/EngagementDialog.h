#ifndef ENGAGEMENTDIALOG_H
#define ENGAGEMENTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "core/EngagementManager.h"

namespace CounterUAS {

class EngagementDialog : public QDialog {
    Q_OBJECT
public:
    explicit EngagementDialog(const AuthorizationRequest& request, QWidget* parent = nullptr);
    
signals:
    void authorized(const QString& operatorId);
    void denied(const QString& reason);
    
private:
    AuthorizationRequest m_request;
    QLabel* m_videoLabel;
    QLabel* m_infoLabel;
};

} // namespace CounterUAS
#endif
