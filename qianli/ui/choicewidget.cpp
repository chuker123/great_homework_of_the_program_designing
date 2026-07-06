#include "choicewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

ChoiceWidget::ChoiceWidget(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void ChoiceWidget::setupUI() {
    // 先设置基础样式
    setStyleSheet("background-color: #1a1a2a; color: white;");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    // 背景图标签（放在最底层）
    QLabel* bgLabel = new QLabel(this);
    bgLabel->setGeometry(0, 0, 1000, 600);
    bgLabel->setScaledContents(true);
    QPixmap bg(":/resources/backgrounds/choice_bg.png");
    if (!bg.isNull()) {
        bgLabel->setPixmap(bg);
    }
    bgLabel->lower();  // 放到最底层
    // 标题
    m_titleLabel = new QLabel();
    m_titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #f39c12;");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_titleLabel);

    // 描述
    m_descLabel = new QLabel();
    m_descLabel->setStyleSheet("font-size: 16px; color: #ecf0f1; line-height: 1.6;");
    m_descLabel->setWordWrap(true);
    m_descLabel->setAlignment(Qt::AlignCenter);
    m_descLabel->setMinimumHeight(150);
    mainLayout->addWidget(m_descLabel);

    // 结果标签（初始隐藏）
    m_resultLabel = new QLabel();
    m_resultLabel->setStyleSheet("font-size: 18px; color: #2ecc71; font-weight: bold; padding: 20px; "
        "background-color: rgba(42, 42, 58, 200); border-radius: 10px;");
    m_resultLabel->setWordWrap(true);
    m_resultLabel->setAlignment(Qt::AlignCenter);
    m_resultLabel->hide();
    mainLayout->addWidget(m_resultLabel);

    // 选项按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(30);

    m_btnA = new QPushButton();
    m_btnA->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; padding: 15px 40px; "
        "font-size: 16px; border-radius: 8px; min-width: 180px; }"
        "QPushButton:hover { background-color: #c0392b; }");
    connect(m_btnA, &QPushButton::clicked, this, &ChoiceWidget::onOptionA);
    btnLayout->addWidget(m_btnA);

    m_btnB = new QPushButton();
    m_btnB->setStyleSheet("QPushButton { background-color: #3498db; color: white; padding: 15px 40px; "
        "font-size: 16px; border-radius: 8px; min-width: 180px; }"
        "QPushButton:hover { background-color: #2980b9; }");
    connect(m_btnB, &QPushButton::clicked, this, &ChoiceWidget::onOptionB);
    btnLayout->addWidget(m_btnB);

    mainLayout->addLayout(btnLayout);

    // 继续按钮（初始隐藏）
    m_continueBtn = new QPushButton("继续");
    m_continueBtn->setStyleSheet("QPushButton { background-color: #2ecc71; color: white; padding: 12px 50px; "
        "font-size: 16px; border-radius: 8px; }"
        "QPushButton:hover { background-color: #27ae60; }");
    m_continueBtn->hide();
    mainLayout->addWidget(m_continueBtn, 0, Qt::AlignCenter);

    mainLayout->addStretch();
}

void ChoiceWidget::setChoiceEvent(const ChoiceEvent& event) {
    m_currentEvent = event;
    m_titleLabel->setText(event.title);
    m_descLabel->setText(event.description);
    m_btnA->setText(event.optionA);
    m_btnB->setText(event.optionB);

    m_resultLabel->hide();
    m_continueBtn->hide();
    m_btnA->show();
    m_btnB->show();
    m_btnA->setEnabled(true);
    m_btnB->setEnabled(true);
}

void ChoiceWidget::onOptionA() {
    m_btnA->setEnabled(false);
    m_btnB->setEnabled(false);
    m_btnA->hide();
    m_btnB->hide();

    m_resultLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #e74c3c; "           
        "font-weight: bold; "
        "padding: 20px; "
        "background-color: rgba(42, 42, 58, 200); "
        "border-radius: 10px;"
    );
    m_resultLabel->setText(m_currentEvent.resultA);
    m_resultLabel->show();

    m_continueBtn->show();
    disconnect(m_continueBtn, nullptr, nullptr, nullptr);
    connect(m_continueBtn, &QPushButton::clicked, [this]() {
        emit choiceMade(true);
        });
}

void ChoiceWidget::onOptionB() {
    m_btnA->setEnabled(false);
    m_btnB->setEnabled(false);
    m_btnA->hide();
    m_btnB->hide();

    m_resultLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #2ecc71; "           
        "font-weight: bold; "
        "padding: 20px; "
        "background-color: rgba(42, 42, 58, 200); "
        "border-radius: 10px;"
    );
    m_resultLabel->setText(m_currentEvent.resultB);
    m_resultLabel->show();

    m_continueBtn->show();
    disconnect(m_continueBtn, nullptr, nullptr, nullptr);
    connect(m_continueBtn, &QPushButton::clicked, [this]() {
        emit choiceMade(false);
        });
}