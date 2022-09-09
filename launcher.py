import time
import subprocess
import os

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

largeFont = QFont("Arial", 30)

def runMainApplication(projectPath: str) -> None:
    subprocess.run(["OpenGL.exe", projectPath])

def main():
    app = QApplication([])
    size = app.primaryScreen().size()

    window = QWidget()
    window.setFixedSize(size * 0.5)

    layout = QHBoxLayout()

    openRecentLabel = QLabel("Open Recent")
    openRecentLabel.setFont(largeFont)

    recentProjectsList = QListWidget()


    #change
    projectDir = "projects"
    projectFiles = [file for file in os.listdir(projectDir) if os.path.isfile(os.path.join(projectDir, file))]    

    for file in projectFiles:
        recentProjectsList.addItem(file)

    newProjectButton = QPushButton("Create a new\nproject")
    newProjectButton.setFont(largeFont)

    # []() { runMainApplication(""); }
    newProjectButton.pressed.connect(lambda : runMainApplication(""))

    layout.addWidget(openRecentLabel)
    layout.addWidget(recentProjectsList)
    layout.addWidget(newProjectButton)

    window.setLayout(layout)
    window.show()
    app.exec()

if __name__ == "__main__":
    main()