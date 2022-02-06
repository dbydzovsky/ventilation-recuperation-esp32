import React from 'react'
import Grid from '@material-ui/core/Grid';
import Slider from '@material-ui/core/Slider';
import {VentilatorIcon} from "../pages/CurrentStatePanel";
import AddIcon from '@material-ui/icons/Add';
import {IconButton, Input, InputAdornment, Tooltip} from "@material-ui/core";
import {makeStyles, Theme} from "@material-ui/core/styles";
import DeleteIcon from '@material-ui/icons/Delete';
import {percentageValidator, temperatureValidator} from "../pages/ConfigurationPage";

export interface Rule {
    tv: number
    p: number
}

interface Props {
    adornment: string
    placeholder: string
    header: string,
    rules: Rule[],
    requiredDifference: number,
    newDefault: number,
    valueValidator: (val:number) =>boolean,
    onChange: (rules: Rule[])=>void,
    validate: (previous: Rule, actual: Rule) => boolean
}

export const TurnOnOffRules = (props: Props) => {
    const value = props.rules ? props.rules : [];
    const setValue = (rules: Rule[]) => {
        let valid = rules.every((rule) => {
            return props.valueValidator(rule.tv) && percentageValidator(rule.p)
        })
        if (valid) {
            props.onChange(rules);
        }
    }
    const classes = useStyles();
    return <div>
        <div className={classes.row}>
            {props.header}
            <IconButton color="primary" aria-label="Přidat pravidlo" onClick={() => {
                setValue([...value, {tv: props.newDefault, p: 50}])
            }}>
                <AddIcon/>
            </IconButton>
        </div>

        {value.map((item, index) => {
            const isError = value
                .filter((_, i) => {
                    return i < index
                })
                .some((previous) => props.validate(previous, item))
            return <div className={classes.row}>
                    <Grid container alignItems="center" className={classes.grid}>
                        <IconButton color="secondary" aria-label="Smazat" onClick={() => {
                            let newArr = [...value]
                            newArr.splice(index, 1)
                            setValue(newArr)
                        }}>
                            <DeleteIcon color={"error"}/>
                        </IconButton>
                        <Tooltip title={isError ? "Hodnota musí být vždy alespoň o " +props.requiredDifference+" rozdílná než předchozí,." : ""}>
                            <Input
                                id="standard-adornment-amount"
                                error={isError}
                                value={item.tv}
                                placeholder={props.placeholder}
                                onChange={(event) => {
                                    item.tv = Number(event.target.value)
                                    setValue([...value])
                                }}
                                className={classes.temperature}
                                endAdornment={<InputAdornment position="start">{props.adornment}</InputAdornment>}
                            />
                        </Tooltip>
                        {/*<Divider orientation={"vertical"} />*/}
                        <div className={classes.percentageRoot}>
                            <VentilatorIcon percentage={item.p}/> {item.p}%
                        </div>
                        <Slider className={classes.percentage} min={0} max={100} step={5} value={item.p}
                                onChange={(event, newValue) => {
                                    item.p = newValue as number
                                    setValue([...value])
                                }} aria-labelledby="continuous-slider"/>
                    </Grid>
            </div>
        })}


    </div>
}

const useStyles = makeStyles((theme: Theme) => ({
    row: {
        display: "flex",
        alignItems: "center",
        justifyContent: "left",
    },
    percentageRoot: {
        width: 100,
        display: "flex",
        alignItems: "center",
        justifyContent: "left",

    },
    grid: {
        width: 'fit-content',
        paddingRight: 30,
        border: `1px solid ${theme.palette.divider}`,
        borderRadius: theme.shape.borderRadius,
        backgroundColor: theme.palette.background.paper,
        color: theme.palette.text.secondary,
        '& svg': {
            margin: theme.spacing(1.5),
        },
        '& hr': {
            margin: theme.spacing(0, 0.5),
        },
    },
    temperature: {
        width: 100,
        textAlign: "center"
    },
    percentage: {
        width: 300
    }
}))