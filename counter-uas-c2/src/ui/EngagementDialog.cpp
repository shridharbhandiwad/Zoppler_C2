#include "ui/EngagementDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

namespace CounterUAS {

EngagementDialog::EngagementDialog(const AuthorizationRequest& request, QWidget* parent)
    : QDialog(parent), m_request(request) {
    setWindowTitle("Engagement Authorization");
    setModal(true);
    resize(400, 300);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    m_videoLabel = new QLabel(this);
    m_videoLabel->setMinimumSize(320, 180);
    m_videoLabel->setStyleSheet("background-color: black;");
    m_videoLabel->setAlignment(Qt::AlignCenter);
    if (!request.videoThumbnail.isNull()) {
        m_videoLabel->setPixmap(QPixmap::fromImage(request.videoThumbnail));
    } else {
        m_videoLabel->setText("No Video");
    }
    layout->addWidget(m_videoLabel);
    
    m_infoLabel = new QLabel(this);
    m_infoLabel->setText(QString("Track: %1\nEffector: %2\nThreat Level: %3\n\n%4")
                            .arg(request.trackId)
                            .arg(request.effectorId)
                            .arg(request.threatLevel)
                            .arg(request.recommendationReason));
    layout->addWidget(m_infoLabel);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    
    QPushButton* authorizeBtn = new QPushButton("AUTHORIZE", this);
    authorizeBtn->setStyleSheet("background-color: green; color: white; font-weight: bold;");
    QPushButton* denyBtn = new QPushButton("DENY", this);
    denyBtn->setStyleSheet("background-color: red; color: white;");
    
    btnLayout->addWidget(denyBtn);
    btnLayout->addWidget(authorizeBtn);
    layout->addLayout(btnLayout);
    
    connect(authorizeBtn, &QPushButton::clicked, this, [this]() {
        emit authorized("OPERATOR");
        accept();
    });
    connect(denyBtn, &QPushButton::clicked, this, [this]() {
        emit denied("Operator denied");
        reject();
    });
}

} // namespace CounterUAS
