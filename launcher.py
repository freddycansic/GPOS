import time
import subprocess
import os

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

APPLICATION_FILE = "OpenGL.exe"
WINDOW_TITLE = "OpenGL"
PROJECT_DIR = "projects"

largeFont = QFont("Arial", 30)

def runMainApplication(projectPath: str) -> None:
    print(APPLICATION_FILE, projectPath)
    subprocess.run([APPLICATION_FILE, projectPath])

def main():
    # preliminary checks
    
    if (not os.path.isfile(APPLICATION_FILE)):
        print(f"Application file \"{APPLICATION_FILE}\" not found! Aborting!")
        exit(-1)
    
    if (not os.path.isdir(PROJECT_DIR)):
        os.mkdir(PROJECT_DIR)
    
    app = QApplication([])
    size = app.primaryScreen().size()

    window = QWidget()
    window.setFixedSize(size * 0.5)
    window.setWindowTitle(WINDOW_TITLE)

    mainLayout = QHBoxLayout()
    recentProjLayout = QVBoxLayout()

    openRecentLabel = QLabel("Open recent projects")
    openRecentLabel.setFont(largeFont)
    openRecentLabel.setAlignment(Qt.AlignCenter)

    recentProjectsList = QListWidget()

    # get all projects
    projectFiles = [file for file in os.listdir(PROJECT_DIR) if os.path.isfile(os.path.join(PROJECT_DIR, file))]    

    for file in projectFiles:
        recentProjectsList.addItem(file)

    # if project double clicked, run main app with project name as args
    recentProjectsList.itemDoubleClicked.connect(lambda item : runMainApplication("\"" + item.text() + "\""))

    newProjectButton = QPushButton("Create a new\nproject")
    newProjectButton.setFont(largeFont)

    newProjectButton.pressed.connect(lambda : runMainApplication(""))

    recentProjLayout.addWidget(openRecentLabel)
    recentProjLayout.addWidget(recentProjectsList)
    mainLayout.addLayout(recentProjLayout)
    mainLayout.addWidget(newProjectButton)

    window.setLayout(mainLayout)
    window.show()
    app.exec()

if __name__ == "__main__":
    main()
