import React from "react";
import {Tooltip} from "@material-ui/core";
import {makeStyles} from "@material-ui/core/styles";
import {Sensor} from "../model/state";

export interface SensorProps {
    icon: any,
    obj: Sensor,
    unit: string,
    label: string,
    transform?: (val: number) => number,
    precission?: number
}

export function SensorValue(props: SensorProps) {
    const classes = useStyles();
    // @ts-ignore
    let value = props.obj.v;
    value = value ? value : 0
    if (props.transform) {
        value = props.transform(value)
    }
    value = roundMe(value, props.precission)
    // @ts-ignore
    let valueAvg = props.obj.avg as number | undefined
    valueAvg = valueAvg ? valueAvg : 0
    if (props.transform) {
        valueAvg = props.transform(valueAvg)
    }
    valueAvg = roundMe(valueAvg, props.precission)
    // @ts-ignore
    let valueErr = props.obj.err
    valueErr = valueErr ? valueErr : 0
    // @ts-ignore
    let valueWarn = props.obj.warn as number | undefined
    valueWarn = valueWarn ?  valueWarn : 0
    let title = valueWarn + " varování a " + valueErr + " chyb"
    return <Tooltip title={title}>
        <div className={classes.flex}>
            {props.icon} {props.label}: {value}{props.unit} (průměr {valueAvg}{props.unit})
        </div>
    </Tooltip>
}

const useStyles = makeStyles(() => {
    return {
        flex: {
            display: "flex",
            alignItems: "center",
            justifyContent: "left",
        }
    }
})


export function roundMe(value: number, precission: number | undefined): number {
    if (precission == undefined) {
        precission = 2;
    }
    let x = Math.pow(10, precission)
    return Math.round(value * x) / x
}
