import React from 'react'
import {TempPressSensor} from "../model/state";
import {makeStyles} from "@material-ui/styles";
import WhatshotIcon from '@material-ui/icons/Whatshot';
import {SensorValue} from "./SensorValue";

export function TempPressSensorComp(props: {data: TempPressSensor, label: string}) {
    const classes = useStyles();
    return <div className={classes.root}>
        <b>{props.label}</b>
        <SensorValue icon={<WhatshotIcon/>} obj={props.data} property={"t"} unit={"°C"} label={"Teplota"} />
    </div>
}
const useStyles = makeStyles({
    root: {
        textAlign: "left",
        margin: 10
    }
})