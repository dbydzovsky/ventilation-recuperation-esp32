
const baseUrl = "https://raw.githubusercontent.com/dbydzovsky/ventilation-recuperation-esp32/main/images/"
const githubImage = (name: string): string => {
    return baseUrl + name
}
export const Images = {
    board: {src: githubImage("board.png")},
    board2: {src: githubImage("board2.png")},
    boardPic: {src: githubImage("board-pic.jpg")},
    conector: {src: githubImage("konektor.jpg")},
    sht20: {src: githubImage("sht20.png")},
    pinout: {src: githubImage("pinout.png")},
    rj45: {src: githubImage("rj45pinout.png")},
}
