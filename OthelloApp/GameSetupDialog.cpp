#include "GameState.h"
#include "Constants.h"
#include "GameSetupDialog.h"

GameSetupDialog::GameSetupDialog(QWidget* parent) : QDialog(parent) {
        setWindowTitle("Othello - Game Setup");
        setModal(true);
        
        auto* layout = new QVBoxLayout(this);
        
        layout->addWidget(new QLabel("Choose Game Mode:"));
        
        gameMode = new QButtonGroup(this);
        
        auto* humanVsHuman = new QRadioButton("Human vs Human");
        auto* humanVsBot = new QRadioButton("Human vs Bot");
        auto* botVsBot = new QRadioButton("Bot vs Bot");
        
        humanVsHuman->setChecked(true); 
        
        gameMode->addButton(humanVsHuman, 1);
        gameMode->addButton(humanVsBot, 2);
        gameMode->addButton(botVsBot, 3);
        
        layout->addWidget(humanVsHuman);
        layout->addWidget(humanVsBot);
        layout->addWidget(botVsBot);
        
        layout->addWidget(new QLabel("\nBot Difficulty (if applicable):"));
        
        bot1Difficulty = new QComboBox();
        bot1Difficulty->addItem("Easy", static_cast<int>(easy));
        bot1Difficulty->addItem("Medium", static_cast<int>(medium));
        bot1Difficulty->addItem("Hard", static_cast<int>(hard));
        
        bot2Difficulty = new QComboBox();
        bot2Difficulty->addItem("Easy", static_cast<int>(easy));
        bot2Difficulty->addItem("Medium", static_cast<int>(medium));
        bot2Difficulty->addItem("Hard", static_cast<int>(hard));
        
        layout->addWidget(new QLabel("Bot 1 (or single bot) difficulty:"));
        layout->addWidget(bot1Difficulty);
        
        bot2Label = new QLabel("Bot 2 difficulty:");
        layout->addWidget(bot2Label);
        layout->addWidget(bot2Difficulty);
        
        bot2Label->hide();
        bot2Difficulty->hide();
        
        connect(gameMode, QOverload<int>::of(&QButtonGroup::idClicked),
                this, &GameSetupDialog::onGameModeChanged);
        
        auto* buttonLayout = new QHBoxLayout();
        auto* okButton = new QPushButton("Start Game");
        auto* cancelButton = new QPushButton("Cancel");
        
        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
        
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        
        layout->addLayout(buttonLayout);
    }
