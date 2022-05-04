
const baseUrl = "https://raw.githubusercontent.com/dbydzovsky/ventilation-recuperation-esp32/main/images/"
const githubImage = (name: string): string => {
    return baseUrl + name
}
const screenImage = (name: string): string => {
    return baseUrl + "screens/" + name
}
export const Images = {
    board: {src: githubImage("board.png")},
    board2: {src: githubImage("board2.png")},
    boardPic: {src: githubImage("board-pic.jpg")},
    conector: {src: githubImage("konektor.jpg")},
    sht20: {src: githubImage("sht20.png")},
    pinout: {src: githubImage("pinout.png")},
    rj45: {src: githubImage("rj45pinout.png")},
    alarmScreen: {src: screenImage("alarmScreen.png")},
    debug: {src: screenImage("debug.png")},
    disableScreen: {src: screenImage("disableScreen.png")},
    historyCo2Screen: {src: screenImage("historyCo2Screen.png")},
    inhouseScreen: {src: screenImage("inhouseScreen.png")},
    mainscreen1: {src: screenImage("mainscreen1.png")},
    mainScreen2: {src: screenImage("mainScreen2.png")},
    manualScreen: {src: screenImage("manualScreen.png")},
    outscreen: {src: screenImage("outscreen.png")},
    party: {src: screenImage("party.png")},
    tempHistory: {src: screenImage("tempHistory.png")},
}
