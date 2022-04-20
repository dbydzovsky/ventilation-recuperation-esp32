
import React from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {Images} from "../images/Images";
import Grid from "@material-ui/core/Grid";

export interface Props {

}

export function InstallationPage(props: Props) {
    const classes = useStyles()


    return <div className={classes.root}>
        <h1>Instalační návod</h1>
        <p>

        </p>

        <Grid container>

            <Grid item xs={12}>
                <h2>Sensor</h2>
                {/* @ts-ignore*/}
                <img src={Images.sht20.src} inline/>
            </Grid>
            <Grid item xs={12}>
                <h2>Pinout Wemos D1 ESP32 mini</h2>
                {/* @ts-ignore*/}
                <img src={Images.pinout.src} inline/>
            </Grid>
            <Grid item xs={12}>
                <h2>Deska</h2>
            </Grid>
            <Grid item xs={12}>
                {/* @ts-ignore*/}
                <img src={Images.boardPic.src} inline/>
            </Grid>
            <Grid item xs={12}>
                {/* @ts-ignore*/}
                <img src={Images.board.src} inline/>
            </Grid>
            <Grid item xs={12}>
                <h1>RJ45 kabely</h1>
                <p>RJ45 kabely musí používat standard RJ45-A</p>
                {/* @ts-ignore*/}
                <img src={Images.conector.src} inline/>
                {/* @ts-ignore*/}
                <img src={Images.rj45.src} inline/>
            </Grid>
            <Grid item xs={6}>
                <Wires title={"Display Konektor"} desc={["SCL", "GND", "27 (reserved)", "32 (reserved)", "25 (reserved)", "5V", "3.3V", "SDA"]}/>
            </Grid>
            <Grid item xs={6}>
                <Wires title={"Sensor Konektor"} desc={["SCL", "GND", "36 - RX - co2", "5 - TX - co2", "26 - Button", "5V", "3.3V", "SDA"]}/>
            </Grid>
            <Grid item xs={12}>
                <h2>Zdrojový kód</h2>
                <p>Zdrojový kód je veřejný. <a href={"https://github.com/dbydzovsky/ventilation-recuperation-esp32"} target={"_blank"}>Github</a></p>
            </Grid>
        </Grid>
    </div>
}

export function Wires(props:{title: string, desc: string[]}) {
    return <>
        <h2>{props.title}</h2>
        <table>
        <WireRow stripped color={"green"}>{props.desc[0]}</WireRow>
        <WireRow color={"green"}>{props.desc[1]}</WireRow>
        <WireRow stripped color={"orange"}>{props.desc[2]}</WireRow>
        <WireRow color={"blue"}>{props.desc[3]}</WireRow>
        <WireRow stripped color={"blue"}>{props.desc[4]}</WireRow>
        <WireRow color={"orange"}>{props.desc[5]}</WireRow>
        <WireRow stripped color={"brown"}>{props.desc[6]}</WireRow>
        <WireRow color={"brown"}>{props.desc[7]}</WireRow>
    </table></>
}

export function WireRow(props: {children: any, stripped?: boolean, color: "brown" |"green" |"orange"|"blue"}) {
    const classes = useStyles()
    const style = {background: props.color}
    return <tr>
        <td className={classes.row}>
            <div className={classes.squeare} style={style}> </div>
            <div className={classes.squeare} style={props.stripped ? {} : style}> </div>
            <div className={classes.squeare} style={{background: props.color}}> </div>
            <div className={classes.squeare} style={props.stripped ? {} : style}> </div>
            <div className={classes.squeare} style={{background: props.color}}> </div>
            <div className={classes.squeare} style={props.stripped ? {} : style}> </div>
            <div className={classes.squeare} style={{background: props.color}}> </div>
        </td>
        <td>{props.children}</td>
    </tr>
}

const useStyles = makeStyles((theme: Theme) => {
    return {
        root: {

        },
        squeare: {
            width: 20,
            height: 20
        },
        row: {
            border: "1px solid black",
            display: "inline-flex"
        }
    }
})