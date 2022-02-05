import React, {useEffect} from 'react'
import {useActions} from "../actions";
import * as TrialActions from "../actions/trial";
import Slider from '@material-ui/core/Slider';
import {makeStyles} from "@material-ui/core/styles";
import Grid from "@material-ui/core/Grid";
import {VentilatorIcon} from "./CurrentStatePanel";
import {FormControl, FormControlLabel, FormLabel, Paper, Radio, RadioGroup} from "@material-ui/core";
import {useSelector} from "react-redux";
import {RootState} from "../reducers";
import Switch from '@material-ui/core/Switch';

const useStyles = makeStyles((theme) => {
    return {
        root: {
            width: "100%",
            [theme.breakpoints.up("md")]: {
                width: "calc(100% - 0px)",
            }
        },
        paper: {
            padding: 20,
            margin: 20
        }
    }
});
export function TrialPage() {
    const trialAction = useActions(TrialActions);
    const classes = useStyles();
    const [power, setVentilator] = React.useState(0);
    const [duration, setDuration] = React.useState("60000");
    const [enabled, setEnabled] = React.useState(false);

    const currentState = useSelector((state: RootState) => state.state);
    useEffect(() => {
        if (currentState.trial) {
            setEnabled(currentState.trial.enabled);
            setVentilator(currentState.trial.ventilator ? currentState.trial.ventilator : 0);
            setDuration(currentState.trial.duration ? currentState.trial.duration + "" : "60000");
        } else {
            setEnabled(false);
        }
    }, [currentState.trial]);
    const handlePowerChange = (event: any, newPower: number|number[]) => {
        setVentilator(newPower as number)
    };
    const handlePowerChangeCommited = (event: any, newPower: number|number[]) => {
        setVentilator(newPower as number)
        trialAction.changeTrial({power: newPower, duration: duration, on: true})
    };
    const handleDurationChange = (event: any, newDuration: string) => {
        setDuration(newDuration);
        trialAction.changeTrial({power: power, duration: Number(newDuration), on: true})
    };
    const activation = (event: React.ChangeEvent<HTMLInputElement>, checked: boolean) => {
        if (checked) {
            trialAction.changeTrial({power: power, duration: duration, on: true})
        } else {
            trialAction.changeTrial({power: 0, duration: 0, on: false})
        }
    }
    return <div className={classes.root} >
        <Paper className={classes.paper}>
            <Grid container spacing={2}>
                <Grid item xs={12}>
                    <FormLabel component="legend">Spustit/vypnout manuální ovládání</FormLabel>
                </Grid>
                <Grid item xs>
                    <Switch
                        checked={enabled}
                        onChange={activation}
                        color="secondary"
                        name="checkedB"
                        inputProps={{ 'aria-label': 'secondary checkbox' }}
                    />
                </Grid>
            </Grid>
        </Paper>
        <Paper className={classes.paper}>
                <FormControl component="fieldset">
                    <Grid container spacing={2} direction={"column"}>
                        <Grid item>
                            <FormLabel component="legend">Doba trvání manuálního spuštění</FormLabel>
                        </Grid>
                        <Grid item>
                            <RadioGroup aria-label={"trvani"} name="duration" value={duration} onChange={handleDurationChange}>
                                <Grid container spacing={2} direction={"row"}>
                                    <FormControlLabel disabled={!enabled} value="60000" control={<Radio />} label="1 minuta" />
                                    <FormControlLabel disabled={!enabled} value="120000" control={<Radio />} label="2 minuty" />
                                    <FormControlLabel disabled={!enabled} value="300000" control={<Radio />} label="5 minut" />
                                    <FormControlLabel disabled={!enabled} value="600000" control={<Radio />} label="10 minut" />
                                    <FormControlLabel disabled={!enabled} value="1800000" control={<Radio />} label="30 minut" />
                                    <FormControlLabel disabled={!enabled} value="3600000" control={<Radio />} label="1 hodina" />
                                    <FormControlLabel disabled={!enabled} value="7200000" control={<Radio />} label="2 hodiny" />
                                    <FormControlLabel disabled={!enabled} value="21600000" control={<Radio />} label="6 hodin" />
                                    <FormControlLabel disabled={!enabled} value="86400000" control={<Radio />} label="1 den" />
                                </Grid>
                            </RadioGroup>
                        </Grid>
                    </Grid>
                </FormControl>
            <br/>
            <br/>
            <br/>
            <Grid container spacing={2}>
                <Grid item xs={12}>
                    <FormLabel component="legend">Nastavení výkonu (aktuálně { power }%)</FormLabel>
                </Grid>
                <Grid item>
                    <VentilatorIcon disabled={!enabled} percentage={0}/>
                </Grid>
                <Grid item xs>
                    <Slider color={"secondary"} disabled={!enabled} value={power} step={5} max={100} min={0} onChange={handlePowerChange} onChangeCommitted={handlePowerChangeCommited} aria-labelledby="continuous-slider" />
                </Grid>
                <Grid item>
                    <VentilatorIcon disabled={!enabled} percentage={100}/>
                </Grid>
            </Grid>
        </Paper>
    </div>
}