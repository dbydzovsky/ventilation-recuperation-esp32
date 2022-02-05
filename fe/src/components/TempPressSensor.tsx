import React from 'react'
import {Sensor} from "../model/state";
import {makeStyles} from "@material-ui/styles";
import WhatshotIcon from '@material-ui/icons/Whatshot';
import {SensorValue} from "./SensorValue";

export function TempCo2SensorComp(props: {temp: Sensor, co2: Sensor, label: string}) {
    const classes = useStyles();
    return <div className={classes.root}>
        <b>{props.label}</b>
        <SensorValue icon={<WhatshotIcon/>} obj={props.temp} unit={"°C"} label={"Teplota"} />
        <SensorValue icon={<WhatshotIcon/>} obj={props.co2} unit={"PPM"} label={"CO2"} />
    </div>
}
const useStyles = makeStyles({
    root: {
        textAlign: "left",
        margin: 10
    }
})