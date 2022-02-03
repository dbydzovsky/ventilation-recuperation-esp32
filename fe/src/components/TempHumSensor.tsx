import React from 'react'
import OpacityIcon from '@material-ui/icons/Opacity';
import {makeStyles} from "@material-ui/styles";
import WhatshotIcon from '@material-ui/icons/Whatshot';
import {TempHumSensor} from "../model/state";
import {roundMe, SensorValue} from "./SensorValue";

function computeDewPoint(h: number, t: number) {
    let e = 2.71828;
    let powResult = Math.pow(e, (17.67 * t) / (243.5 + t));
    let logResult = Math.log((h / 100) * powResult);
    return (243.5 * logResult) / (17.67 - logResult)
}
export function TempHumSensorComp(props: { data: TempHumSensor, label: string }) {
    const classes = useStyles();
    let h = props.data.hAvg;
    let t = props.data.tAvg;
    let dewPoint = "?"
    if (h && t) {
        dewPoint = roundMe(computeDewPoint(h, t), 2) + "";
    }
    return <div className={classes.root}>
        <b>{props.label}</b> <i>(Rosný bod: {dewPoint} °C)</i>
        <SensorValue icon={<WhatshotIcon/>} obj={props.data} property={"t"} unit={"°C"} label={"Teplota"}/>
        <SensorValue icon={<OpacityIcon/>} obj={props.data} precission={0} property={"h"} unit={"%"} label={"Vlhkost"}/>
    </div>
}


const useStyles = makeStyles({
    root: {
        textAlign: "left",
        margin: 10
    }
})