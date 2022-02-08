import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import Grid from "@material-ui/core/Grid";
import {useDispatch, useSelector} from "react-redux";
import PowerSettingsNewIcon from '@material-ui/icons/PowerSettingsNew';
import PhotoFilterIcon from '@material-ui/icons/PhotoFilter';
import WarningIcon from "@material-ui/icons/Warning";
import Button from "@material-ui/core/Button";
import {InputLabel, Paper} from "@material-ui/core";
import {cleanFilter, FanType} from "../actions/filter";
import {RootState} from "../reducers";
import {FilterState} from "../model/state";

export function FilterPage() {
    const ventilatorFilter = useSelector((state: RootState) => state.state.filterVentilator);
    const recuperationFilter = useSelector((state: RootState) => state.state.filterRecuperation);

    return <Grid container>
            <FilterShowcase filter={FanType.ventilator}
                            state={ventilatorFilter}
                            title={"Filtr ventilátoru"}
            />
            <FilterShowcase filter={FanType.recuperation}
                            state={recuperationFilter}
                            title={"Filtr rekuperace"}
            />
        </Grid>
}

export interface FilterShowcaseProps {
    filter: FanType
    state: FilterState
    title: string
}
export function FilterShowcase(props: FilterShowcaseProps)  {
    const classes = useStyles();
    const dispatch = useDispatch();
    const days = Math.round(props.state.remainingMinutes / 60 / 24);
    return <Grid item xs={12} sm={12} md={6}>
        <Paper className={classes.properties}>
            <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                <PhotoFilterIcon/>
                {props.title}
            </InputLabel>
            {props.state.needCleaning && <div style={{color: "orange"}}>
                <p>
                    <WarningIcon htmlColor={"orange"}/> Filtr by měl být vyčištěný
                </p>
            </div>}
            <p>
                {props.state.needCleaning && <>
                    Filtr už přesluhuje. Je v provozu {Math.abs(days)} dnů/dní navíc, než by měl.
                </>}
                {!props.state.needCleaning && <>
                    Filter je v pořádku. Zbývá ještě {days} dnů/dní čistého času provozu.
                </>}
            </p>
            <div className={classes.box}>
                <Button variant={props.state.needCleaning ? "contained" : "outlined"}
                        color={props.state.needCleaning ? "secondary" : "primary"}
                        startIcon={<PowerSettingsNewIcon/>}
                        onClick={() => {
                            dispatch(cleanFilter({filter: props.filter}))
                        }}>
                    Vyčistil jsem filter
                </Button>
            </div>
        </Paper>
    </Grid>
}

const useStyles = makeStyles(() => {
    return {
        label: {
            display: "flex",
            alignItems: "center",
        },
        box: {
            margin: 20
        },
        properties: {
            margin: 20,
            padding: 20
        },
        root: {
            margin: 20
        },
        row: {
            textAlign: "center"
        }
    }
})