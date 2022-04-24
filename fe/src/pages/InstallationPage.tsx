
import React from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {Images} from "../images/Images";
import Grid from "@material-ui/core/Grid";
import WarningIcon from '@material-ui/icons/Warning';
export interface Props {

}

export function InstallationPage(props: Props) {
    const classes = useStyles()

    return <div className={classes.root}>
        <Grid container>
            <p><WarningIcon htmlColor={"orange"}/>
                Před jakoukoli montáží je třeba zařízení odpojit od přivodu elektrické energie.
                Při jakékoliv montáží je třeba pečlivě sledovat značení na desce.
                Nejsou povolené žádné vlastní úpravy, které by mohli ohrozit funkčnost zařízení.
            </p>
            <Grid item xs={12}>
                <h2>Sensor</h2>
                <img className={classes.image} width={300} src={Images.sht20.src}/>
            </Grid>
            <Grid item xs={12}>
                <h2>Pinout Wemos D1 ESP32 mini</h2>
                <img  className={classes.image} width={700} src={Images.pinout.src} />
            </Grid>
            <Grid item xs={12}>
                <h2>Deska</h2>
            </Grid>
            <Grid item xs={12}>
                <img  className={classes.image} width={700} src={Images.boardPic.src}/>
            </Grid>
            <Grid item xs={12}>
                <img className={classes.image} width={700} src={Images.board.src}/>
            </Grid>
            <Grid item xs={12}>
                <img className={classes.image} width={700} src={Images.board2.src}/>
            </Grid>

            <Grid item xs={12}>
                <h1>RJ45 kabely</h1>
                <p>RJ45 kabely musí používat standard RJ45-A</p>
            </Grid>
            <Grid item xs={12} md={6}>
                <img className={classes.image} width={300} src={Images.rj45.src}/>
            </Grid>
            <Grid item xs={12} md={6}>
                <img className={classes.image} width={300} src={Images.conector.src}/>
            </Grid>
            <Grid item xs={12} md={6}>
                <Wires title={"Display Konektor"} desc={["SCL", "GND", "27 (reserved)", "32 (reserved)", "25 (reserved)", "5V", "3.3V", "SDA"]}/>
            </Grid>
            <Grid item xs={12} md={6}>
                <Wires title={"Sensor Konektor"} desc={["SCL", "GND", "36 - RX - co2", "5 - TX - co2", "26 - Button", "5V", "3.3V", "SDA"]}/>
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
        image:{
            maxWidth: "100%"
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