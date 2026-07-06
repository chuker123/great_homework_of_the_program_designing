#include "levelmapwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QGraphicsDropShadowEffect>

LevelMapWidget::LevelMapWidget(QWidget* parent) : QWidget(parent) {
    m_levelNames = QStringList() << "东岭关" << "洛阳" << "汜水关" << "荥阳" << "滑州";
    setupUI();
}

void LevelMapWidget::setupUI() {
    // 先创建外层垂直布局（控制整体垂直居中）
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // 去掉默认边距

    // 背景图标签（保持原逻辑）
    QLabel* bgLabel = new QLabel(this);
    bgLabel->setGeometry(0, 0, 1200, 700);
    bgLabel->setScaledContents(true);
    QPixmap bg(":/resources/backgrounds/map_bg.png");
    if (!bg.isNull()) {
        bgLabel->setPixmap(bg);
    }
    bgLabel->lower();

    // 中间容器：放标题、描述、关卡按钮（用于控制居中）
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(15);
    contentLayout->setContentsMargins(30, 20, 30, 20);

    // 标题
    QLabel* title = new QLabel("千里走单骑 - 关卡选择");
    title->setStyleSheet("font-size: 38px; font-weight: bold; color: #f39c12;"); // 标题颜色保持金色
    title->setAlignment(Qt::AlignCenter);

    //黑色描边
    QGraphicsDropShadowEffect *titleShadow = new QGraphicsDropShadowEffect(this);
    titleShadow->setColor(Qt::black); // 描边颜色（黑色）
    titleShadow->setBlurRadius(2);    // 模糊程度（数值越小越像描边）
    titleShadow->setOffset(1, 1);     // 描边偏移（x/y各1px）
    title->setGraphicsEffect(titleShadow);

    contentLayout->addWidget(title);
    contentLayout->addSpacing(5);

    // 描述（保持原逻辑）
    QLabel* desc = new QLabel("建安五年，关羽得知刘备下落，护送二位嫂嫂千里寻兄...\n"
                              "过五关斩六将，一路艰险！");
    desc->setStyleSheet("font-size: 20px; color: #f8f5f0;");
    desc->setAlignment(Qt::AlignCenter);
    desc->setWordWrap(true);
    QGraphicsDropShadowEffect *descShadow = new QGraphicsDropShadowEffect(this);
    descShadow->setColor(Qt::black);
    descShadow->setBlurRadius(1);
    descShadow->setOffset(0.5, 0.5);
    desc->setGraphicsEffect(descShadow);
    contentLayout->addWidget(desc);
    contentLayout->addSpacing(20);

    // 关卡按钮（保持原逻辑，已通过levelsLayout->setAlignment(Qt::AlignCenter)水平居中）
    QHBoxLayout* levelsLayout = new QHBoxLayout();
    levelsLayout->setSpacing(15);
    levelsLayout->setAlignment(Qt::AlignCenter); // 按钮水平居中

    for (int i = 0; i < m_levelNames.size(); ++i) {
        QPushButton* btn = new QPushButton(m_levelNames[i]);
        btn->setFixedSize(120, 80);

        connect(btn, &QPushButton::clicked, [this, i]() {
            emit levelSelected(i);
        });

        levelsLayout->addWidget(btn);
        m_levelButtons.append(btn);

        if (i < m_levelNames.size() - 1) {
            QLabel* arrow = new QLabel("→");
            arrow->setStyleSheet("font-size: 24px; color: #f39c12;");
            levelsLayout->addWidget(arrow);
        }
    }

    contentLayout->addLayout(levelsLayout);

    // 外层布局：上下加stretch，让contentWidget垂直居中
    mainLayout->addStretch(1);
    mainLayout->addWidget(contentWidget);
    mainLayout->addStretch(1);

    updateButtonStates();
}

void LevelMapWidget::setCurrentLevel(int levelIndex) {
    m_currentLevel = levelIndex;
    updateButtonStates();
}

void LevelMapWidget::setLevelsCleared(int count) {
    m_clearedLevels = count;
    updateButtonStates();
}

void LevelMapWidget::updateButtonStates() {
    for (int i = 0; i < m_levelButtons.size(); ++i) {
        QPushButton* btn = m_levelButtons[i];

        if (i < m_clearedLevels) {
            btn->setStyleSheet("QPushButton { background-color: #27ae60; color: white; font-size: 14px; "
                "font-weight: bold; border-radius: 10px; border: 2px solid #2ecc71; }");
            btn->setText(m_levelNames[i] + "\n[已通关]");
            btn->setEnabled(true);
        }
        else if (i == m_currentLevel) {
            btn->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; font-size: 14px; "
                "font-weight: bold; border-radius: 10px; border: 2px solid #c0392b; }");
            btn->setText(m_levelNames[i] + "\n[当前]");
            btn->setEnabled(true);
        }
        else {
            btn->setStyleSheet("QPushButton { background-color: #2c3e50; color: #7f8c8d; font-size: 14px; "
                "font-weight: bold; border-radius: 10px; border: 2px solid #34495e; }");
            btn->setText(m_levelNames[i] + "\n[未解锁]");
            btn->setEnabled(false);
        }
    }
}

