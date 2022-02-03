import React, {useState} from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import { styled } from '@material-ui/styles';
import Box from '@material-ui/core/Box';
import Typography from '@material-ui/core/Typography';
import Slider from '@material-ui/core/Slider';
import SwapHorizIcon from '@material-ui/icons/SwapHoriz';
import {Grid, IconButton} from "@material-ui/core";

export interface Props {
    start: number
    end: number
    color: "orange" | "blue"
    onChange: (start: number, end: number) => void
}

var theMonths = ["Leden", "Únor", "Březen", "Duben", "Květen",
    "Červen", "Červenec", "Srpen", "Září", "Říjen", "Listopad", "Prosinec"];

const marks = theMonths.map((month,index) => {
    return {
        value: (365/11) * index,
        label: month
    }
})
function dateFromDay(day: number){
    var date = new Date(2009, 0);
    const options = { month: 'numeric', day: 'numeric' };
    return new Date(date.setDate(day)).toLocaleDateString("cs-cz", options).replace(" ", "")
}
export function Range(props: Props) {
    const classes = useStyles()
    const invert = () => {
        props.onChange(props.end, props.start)
    }
    const inverted = props.start > props.end;
    return <div className={classes.root} style={{color: "red !important"}}>
                <Typography id="track-inverted-range-slider" gutterBottom>
                    Prohodit <IconButton>
                        <SwapHorizIcon onClick={invert} />
                    </IconButton>
                </Typography>
                <Slider
                    className={props.color === "orange" ?  classes.orange : classes.blue}
                    track={inverted ? "inverted" : "normal"}
                    aria-labelledby="track-inverted-range-slider"
                    value={[props.start, props.end].sort()}
                    min={1}
                    max={365}
                    valueLabelDisplay="on"
                    valueLabelFormat={(value: number) => dateFromDay(value)}
                    color={"primary"}
                    onChange={(event, value) => {
                        const [start, end] = value as number[]
                        if (inverted) {
                            props.onChange(end, start)
                        } else {
                            props.onChange(start, end)
                        }
                    }}
                />
    </div>
}

const useStyles = makeStyles((theme: Theme) => {
    return {
        root: {

        },
        orange: {
            color: "orange"
        },
        blue: {
            color: "lightskyblue"
        },
    }
})