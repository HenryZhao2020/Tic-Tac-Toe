#include "Dialog.h"
#include "Attr.h"
#include "Board.h"
#include "GameUtil.h"
#include "Game.h"
#include "GameBar.h"
#include "ScoreBoard.h"

#include <QFile>
#include <QFrame>
#include <QGroupBox>
#include <QProcess>

Dialog::Dialog(Game *game, const QIcon &icon, const QString &title)
    : QDialog(game), game{game} {
    setWindowIcon(icon);
    setWindowTitle(title);
    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    auto buttonFrame = new QFrame(this);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(buttonFrame, 0, Qt::AlignCenter);

    buttonLayout = new QHBoxLayout(buttonFrame);
    buttonLayout->setSpacing(10);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    okButton = new QPushButton(tr("OK"), this);
    connect(okButton, &QPushButton::clicked, this, &Dialog::close);
    buttonLayout->addWidget(okButton);
}

Dialog::~Dialog() {}

void Dialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        deleteLater();
    }

    QDialog::keyPressEvent(event);
}

SettingsDialog::SettingsDialog(Game *game)
    : Dialog(game, IconUtil::load(":/icons/Settings.svg"), tr("Settings")) {
    auto gameGroup = newGroup(tr("Game"));
    addCheckBox(gameGroup, tr("Two Player Mode"), Attr::getSettings().twoPlayer);
    addCheckBox(gameGroup, tr("Enable Visual Effects"), Attr::getSettings().animated);
    addCheckBox(gameGroup, tr("Display Scores on Bottom"), Attr::getSettings().showScores);

    langBox = new QComboBox(this);
    langBox->addItems(Lang::getLangNames());
    langBox->setCurrentIndex(Attr::getSettings().lang);

    auto langGroup = newGroup(tr("Language"));
    langGroup->addWidget(langBox);

    resetBox = new QComboBox(this);
    resetBox->addItem(tr("Don't Reset"));
    resetBox->addItem(tr("Reset Everything"));
    resetBox->addItem(tr("Reset Settings"));
    resetBox->addItem(tr("Reset Points"));
    buttonLayout->insertWidget(0, resetBox);
    buttonLayout->insertStretch(1);

    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::applySettings);

    auto cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, &Dialog::close);
    buttonLayout->addWidget(cancelButton);

    setFixedSize(sizeHint());
}

SettingsDialog::~SettingsDialog() {}

QVBoxLayout *SettingsDialog::newGroup(const QString &title) {
    auto group = new QGroupBox(title, this);
    mainLayout->insertWidget(mainLayout->count() - 2, group);

    auto groupLayout = new QVBoxLayout(group);
    groupLayout->setSpacing(10);
    groupLayout->setContentsMargins(20, 20, 20, 20);
    return groupLayout;
}

void SettingsDialog::addCheckBox(QLayout *layout, const QString &text, bool &state) {
    auto box = new QCheckBox(text, this);
    box->setChecked(state);
    boxes.insert(box, &state);
    layout->addWidget(box);
}

void SettingsDialog::applySettings() {
    for (auto it = boxes.begin(); it != boxes.end(); ++it) {
        *it.value() = it.key()->isChecked();
    }

    int resetIndex = resetBox->currentIndex();
    if (resetIndex == 1) {
        qApp->quit();
        QFile::remove("TTT_Data");
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    } else if (resetIndex == 2) {
        Attr::resetSettings();
    } else if (resetIndex == 3) {
        Attr::resetStats();
        game->getScoreBoard()->updateValues();
    }

    game->resumeRound();

    game->getScoreBoard()->updateHeaders();
    game->getScoreBoard()->setVisible(Attr::getSettings().showScores);
    game->centralWidget()->adjustSize();
    game->setFixedSize(game->sizeHint());

    int oldLang = Attr::getSettings().lang;
    int newLang = langBox->currentIndex();
    Attr::getSettings().lang = Lang::Name(newLang);

    if (newLang != oldLang) {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

HelpDialog::HelpDialog(Game *game)
    : Dialog(game, IconUtil::load(":/icons/Help.svg"), tr("Help")) {
    auto tabWidget = new QTabWidget(this);
    tabWidget->addTab(newBrowser(":/help/" + tr("Rules.html")), tr("Rules"));
    tabWidget->addTab(newBrowser(":/help/" + tr("About.html")), tr("About"));
    mainLayout->insertWidget(0, tabWidget);

    setFixedSize(sizeHint());
}

HelpDialog::~HelpDialog() {}

QTextBrowser *HelpDialog::newBrowser(const QString &path) {
    auto browser = new QTextBrowser(this);
    browser->setHtml(FileUtil::readAll(path));
    browser->setOpenExternalLinks(true);
    return browser;
}
