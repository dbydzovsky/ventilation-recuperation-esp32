// {
//     "configuration": [
//     { "id": 1, "maxPower": 40, "author": "typicode" }
// ],
//     "state": [
//     { "id": 1, "body": "some comment", "postId": 1 }
// ],
//     "profile": { "name": "typicode" }
// }
//https://github.com/typicode/json-server

module.exports = () => {
    const data = {
        conf: {
            c: 1.5,
            e: true,
            s: true,
            we: true,
            mh: 80,
            wmit: 255,
            w: true,
            wor: [
                { id: 1, t: 30, p: 50 },
                { id: 2, t: 40, p: 100 },
                { id: 3, t: 50, p: 0 }
            ]
        },
        t: {},
        s: {
            n: "obyvak",
            p: 20,
            tpp: 20,
            tpd: 120000,
            r: 0,
            // a: 146489584,
            a: 11111489584,
            di: false,
            h: 19982,
            de: "sdfsdfs fsfsfs",
            o: {
                t: 21.6856489,
                tWarn: 6,
                tAvg: 21.6851984984,
                tErr: 6,
                h: 70.68498451,
                hAvg: 70.6,
                hErr: 70.6,
                hWarn: 4
            },
            i: {
                t: 21.6856198198,
                tAvg: 21.685618984,
                tErr: 0,
                tWarn: 3
            },
        }
    }

    return data
}