import React, {useEffect} from 'react'
import {useActions} from "../actions";
import * as TrialActions from "../actions/trial";
import {ChangeTrialProps, RecuperationMode} from "../actions/trial";
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
    const [ventilatorPower, setVentilator] = React.useState(0);
    const [recuperationPower, setRecuperation] = React.useState(0);
    const [recuperationMode, setRecuperationMode] = React.useState(RecuperationMode.Recycle);
    const [duration, setDuration] = React.useState("300000");
    const [enabled, setEnabled] = React.useState(false);
    const currentState = useSelector((state: RootState) => state.state);
    useEffect(() => {
        if (currentState.trial) {
            setEnabled(currentState.trial.enabled);
            setVentilator(currentState.trial.ventilator ? currentState.trial.ventilator : 0);
            setRecuperation(currentState.trial.recuperation ? currentState.trial.recuperation : 0);
            setRecuperationMode(currentState.trial.recuperationMode ? currentState.trial.recuperationMode : RecuperationMode.Recycle)
            setDuration(currentState.trial.duration ? currentState.trial.duration + "" : "300000");
        } else {
            setEnabled(false);
        }
    }, [currentState.trial]);
    const handleVentilatorPowerChange = (event: any, newPower: number|number[]) => {
        setVentilator(newPower as number)
    };
    const handleVentilatorPowerChangeCommited = (event: any, newPower: number|number[]) => {
        setVentilator(newPower as number)
        let props: ChangeTrialProps = {
            ventilator: Number(newPower),
            duration: Number(duration),
            recuperationMode: recuperationMode,
            recuperation: recuperationPower,
            on: true
        }
        trialAction.changeTrial(props)
    };
    const handleRecuperationPowerChange = (event: any, newPower: number|number[]) => {
        setRecuperation(newPower as number)
    };
    const handleRecuperationPowerChangeCommited = (event: any, newPower: number|number[]) => {
        setRecuperation(newPower as number)
        let props: ChangeTrialProps = {
            ventilator: ventilatorPower,
            duration: Number(duration),
            recuperationMode: recuperationMode,
            recuperation: Number(newPower),
            on: true
        }
        trialAction.changeTrial(props)
    };
    const handleDurationChange = (event: any, newDuration: string) => {
        setDuration(newDuration);
        let props: ChangeTrialProps = {
            ventilator: ventilatorPower,
            duration: Number(newDuration),
            recuperationMode: recuperationMode,
            recuperation: recuperationPower,
            on: true
        }
        trialAction.changeTrial(props)
    };
    const handleRecuperationModeChange = (event: any, newMode: string) => {
        setRecuperationMode(newMode as any as number);
        let props: ChangeTrialProps = {
            ventilator: ventilatorPower,
            duration: Number(duration),
            recuperationMode: newMode as any as number,
            recuperation: recuperationPower,
            on: true
        }
        trialAction.changeTrial(props)
    };
    const activation = (event: React.ChangeEvent<HTMLInputElement>, checked: boolean) => {
        if (checked) {
            let props: ChangeTrialProps = {
                ventilator: ventilatorPower,
                duration: Number(duration),
                recuperationMode: recuperationMode,
                recuperation: recuperationPower,
                on: true
            }
            trialAction.changeTrial(props)
        } else {
            let props: ChangeTrialProps = {
                recuperationMode: 0,
                ventilator: 0,
                recuperation: RecuperationMode.Recycle,
                duration: 0,
                on: false
            }
            trialAction.changeTrial(props)
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
                                    <FormControlLabel disabled={!enabled} value="300000" control={<Radio />} label="5 minut" />
                                    <FormControlLabel disabled={!enabled} value="600000" control={<Radio />} label="10 minut" />
                                    <FormControlLabel disabled={!enabled} value="1800000" control={<Radio />} label="30 minut" />
                                    <FormControlLabel disabled={!enabled} value="3600000" control={<Radio />} label="1 hodina" />
                                    <FormControlLabel disabled={!enabled} value="7200000" control={<Radio />} label="2 hodiny" />
                                    <FormControlLabel disabled={!enabled} value="10800000" control={<Radio />} label="3 hodiny" />
                                    <FormControlLabel disabled={!enabled} value="21600000" control={<Radio />} label="6 hodin" />
                                    <FormControlLabel disabled={!enabled} value="43200000" control={<Radio />} label="12 hodin" />
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
                    <FormLabel component="legend">Nastavení výkonu ventilátoru (aktuálně { ventilatorPower }%)</FormLabel>
                </Grid>
                <Grid item>
                    <VentilatorIcon disabled={!enabled} percentage={0}/>
                </Grid>
                <Grid item xs>
                    <Slider color={"secondary"} disabled={!enabled} value={ventilatorPower} step={5} max={100} min={0} onChange={handleVentilatorPowerChange} onChangeCommitted={handleVentilatorPowerChangeCommited} aria-labelledby="continuous-slider" />
                </Grid>
                <Grid item>
                    <VentilatorIcon disabled={!enabled} percentage={100}/>
                </Grid>
            </Grid>
            <Grid container spacing={2}>
                <Grid item xs={12}>
                    <FormLabel component="legend">Nastavení výkonu rekuperace (aktuálně { recuperationPower }%)</FormLabel>
                </Grid>
                <Grid item>
                    <VentilatorIcon disabled={!enabled} percentage={100}/>
                </Grid>
                <Grid item xs>
                    <Slider color={"secondary"}
                            disabled={!enabled}
                            value={recuperationPower}
                            step={5}
                            max={100}
                            min={0}
                            onChange={handleRecuperationPowerChange}
                            onChangeCommitted={handleRecuperationPowerChangeCommited}
                            aria-labelledby="continuous-slider"
                    />
                </Grid>
                <Grid item>
                    <VentilatorIcon disabled={!enabled} percentage={100}/>
                </Grid>
            </Grid>
            <FormControl component="fieldset">
                <Grid container spacing={2} direction={"column"}>
                    <Grid item>
                        <FormLabel component="legend">Směr rekuperační jednotky</FormLabel>
                    </Grid>
                    <Grid item>
                        <RadioGroup aria-label={"smer"} name="direction" value={recuperationMode} onChange={handleRecuperationModeChange}>
                            <Grid container spacing={2} direction={"row"}>
                                <FormControlLabel disabled={!enabled} value={RecuperationMode.Recycle} control={<Radio />} label="Recyklovat vzduch" />
                                <FormControlLabel disabled={!enabled} value={RecuperationMode.Inhale} control={<Radio />} label="Pouze dovnitř" />
                                <FormControlLabel disabled={!enabled} value={RecuperationMode.Exhale} control={<Radio />} label="Pouze ven" />
                            </Grid>
                        </RadioGroup>
                    </Grid>
                </Grid>
            </FormControl>
        </Paper>
    </div>
}