import React, {ChangeEvent, useEffect} from 'react'
import {makeStyles} from '@material-ui/core/styles';
import {
    Button,
    FormControl,
    FormControlLabel,
    FormLabel,
    Input,
    InputAdornment,
    InputLabel,
    Paper,
    Radio,
    RadioGroup
} from "@material-ui/core";
import {TurnOnOffRules} from "../components/TurnOnOffRules";
import {useActions} from "../actions";
import * as ConfigurationActions from "../actions/configuration";
import {getConfiguration} from "../actions/configuration";
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import {Configuration, Mode, Rule} from "../model/configuration";
import Grid from "@material-ui/core/Grid";
import TrendingDownIcon from '@material-ui/icons/TrendingDown';
import CompareArrowsIcon from '@material-ui/icons/CompareArrows';
import PanToolIcon from '@material-ui/icons/PanTool';
import BeachAccessIcon from '@material-ui/icons/BeachAccess';
import AcUnitIcon from '@material-ui/icons/AcUnit';
import PersonPinIcon from '@material-ui/icons/PersonPin';
import BrandingWatermarkIcon from '@material-ui/icons/BrandingWatermark';
import ExpansionPanel from '@material-ui/core/ExpansionPanel';
import ExpansionPanelSummary from '@material-ui/core/ExpansionPanelSummary';
import ExpansionPanelDetails from '@material-ui/core/ExpansionPanelDetails';
import Typography from '@material-ui/core/Typography';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import {Range} from "../components/Range";
import WarningIcon from "@material-ui/icons/Warning";
import WbSunnyIcon from '@material-ui/icons/WbSunny';

export const winterOrderValidation = (previous: Rule, actual: Rule) => {
    return previous.t > (actual.t - 3);
}
export const summerOrderValidation = (previous: Rule, actual: Rule) => {
    return previous.t < (actual.t + 3);
}
export const weatherKeys = [
    {key: "lat", label: "Zeměpisná šírka (např. 50.25)", max: 5},
    {key: "lon", label: "Zeměpisná délka (např. 15.39)", max: 5},
    {key: "w", label: "API klíč (max. 32 znaků)", max: 32}
]

export function temperatureValidator(input: string | number) {
    if (input === "0") {
        return true
    } else {
        let candidate = Number(input)
        let valid = !Number.isNaN(candidate)
        if (valid) {
            return candidate > -20 && candidate < 100
        }
        return false
    }
}

export function rangeValidator(min: number, max: number) {
    return function (input: string | number) {
        let candidate = Number(input);
        if (Number.isNaN(candidate)) {
            return false
        }
        return candidate >= min && candidate <= max;
    }
}

export function lengthValidator(max: number) {
    return function (input: string | number) {
        let candidate = input + "";
        return candidate.length <= max;
    }
}

export function noopValidator() {
    return true
}

export function percentageValidator(input: string | number) {
    return rangeValidator(0, 100)(input)
}

function isASCII(str: string) {
    return /^[\x00-\x7F]*$/.test(str) && str.indexOf(" ") < 0;
}

export function ConfigurationPage() {
    const classes = useStyles();
    const configurationAction = useActions(ConfigurationActions);
    const currentState = useSelector((state: RootState) => state.state);
    const state = useSelector((state: RootState) => state.configuration);
    const dispatch = useDispatch();
    const [reloadCounter, setReloadId] = React.useState(0);
    useEffect(() => {
        dispatch(getConfiguration);
    }, [reloadCounter]);
    let initial: Configuration = {
        ass: 100,
        ase: 200,
        aws: 250,
        awe: 50,
        c: 0,
        n: "aName",
        mo: 0,
        td: 90,
        wmit: 240,
        smit: 180,
        wor: [],
        sor: [],
        w: "",
        lat: "",
        lon: "",
        m: {
            f: "",
            k: ""
        }
    }
    const rulesMaxCount = 5;
    const [values, setValues] = React.useState({...state.conf} as Configuration);
    useEffect(() => {
        setValues(state.conf ? state.conf : initial as Configuration)
    }, [state])
    if (!values) {
        return <div></div>
    }
    const maxTrendingDownInvalid = values.td < 60 || values.td > 1800;
    const nameInvalid = !values.n || values.n.length > 20 || !isASCII(values.n);
    const maxInsideTemperatureInvalid = values.wmit < 0 || values.wmit > 1000;
    let winterRulesInvalid = values.wor && values.wor.some((item, index) => {
        return values.wor.filter((_, i) => {
            return i < index;
        }).some((previous) => winterOrderValidation(previous, item))
    })
    let summerRulesInvalid = values.sor && values.sor.some((item, index) => {
        return values.sor.filter((_, i) => {
            return i < index;
        })
            .some((previous) => summerOrderValidation(previous, item))
    })
    let rulesInvalid = winterRulesInvalid
        || summerRulesInvalid
        || (values.wor && values.wor.length > rulesMaxCount)
        || (values.sor && values.sor.length > rulesMaxCount);
    const isOverlapping = (a: number, b: number, c: number, d: number):boolean => {
        if (a < b && c < d) {
            return a < d && c < b;
        }
        if (a > b && c > d) {
            return true;
        }
        const m = 365;
        const w0 = (b - a) % m;
        const w1 = (d - c) % m;
        return (w1 != 0 && ((c - a) % m) < w0) || (w0 != 0 && ((a - c) % m) < w1);
    }
    const winterSummerOverlaps = isOverlapping(values.aws, values.awe, values.ass, values.ase)
    const anyInvalid = maxTrendingDownInvalid || maxInsideTemperatureInvalid || rulesInvalid || nameInvalid || winterSummerOverlaps;
    const handleMonitoringChange = (prop: string) => (event: ChangeEvent<HTMLInputElement>) => {
        let monitoring = {...values.m, [prop]: event.target.value}
        setValues({...values, m: monitoring});
    };
    const handleChange = (prop: string, validator: (value: string) => boolean, transform: (value: string) => any = (it) => it) => (event: ChangeEvent<HTMLInputElement>) => {
        if (validator(event.target.value)) {
            setValues({...values, [prop]: transform(event.target.value)});
        } else {
            setValues({...values});
        }
    };

    const handleRuleChange = (prop: string) => (rules: Rule[]) => {
        setValues({...values, [prop]: rules});
    };

    const handleSave = () => {
        configurationAction.saveConfiguration(values)
        setReloadId(reloadCounter + 1)
    }
    const setMode = (e: ChangeEvent<HTMLInputElement>, value: string) => {
        setValues({...values, mo: Number(value)})
    }
    return <div className={classes.header}>
        <Grid container>
            <Grid item xs={12} sm={6} md={6}>
                <Paper className={classes.properties}>
                    <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                        <BrandingWatermarkIcon/>
                        Název zařízení <i>(bude dostupné jako http://{values.n}/)</i>
                    </InputLabel>
                    <Input
                        error={nameInvalid}
                        id="standard-adornment-name"
                        value={values.n}
                        className={classes.input}
                        onChange={handleChange('n', (it) => it.length <= 20)}
                        startAdornment={<InputAdornment position="start"><PersonPinIcon/></InputAdornment>}
                    />
                </Paper>
            </Grid>
            <Grid item xs={12} md={12} lg={12}>
                <ExpansionPanel className={classes.griditem}>
                    <ExpansionPanelSummary
                        expandIcon={<ExpandMoreIcon/>}
                        aria-controls="panel1a-content"
                        id="panel1a-header"
                    >
                        <Typography>Nastavení monitoringu <i>(https://iotplotter.com)</i></Typography>
                    </ExpansionPanelSummary>
                    <ExpansionPanelDetails>
                        <Grid container>
                            Zařízení každou minutu odesílá hodnoty ze sensorů na webovou stránku iotplotter.com.
                            Využívá k tomu šifrovanou komunikaci.

                            Naměřené hodnoty pomáhají sledovat funkčnost zařízení, ale také sledovat jeho užitečnost.
                            Kromě hodnoty ze sensorů se odesílá také počet restartů, aktuální stav paměti a také počet
                            chybových čtení ze sensorů.
                            {[{key: "f", label: "feed ID"}, {key: "k", label: "Api Key"}].map((item) => {
                                let value = ""
                                if (values.m) {
                                    // @ts-ignore
                                    value = values.m[item.key] || ""
                                }
                                return <Grid item xs={12} md={6} lg={4} key={item.key}>
                                    <Paper className={classes.properties}>
                                        <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                                            {item.label}
                                        </InputLabel>
                                        <Input
                                            id="standard-adornment-name"
                                            value={value}
                                            className={classes.input}
                                            onChange={handleMonitoringChange(item.key)}
                                            startAdornment={<InputAdornment
                                                position="start"><PersonPinIcon/></InputAdornment>}
                                        />
                                    </Paper>

                                </Grid>
                            })}
                        </Grid>

                    </ExpansionPanelDetails>
                </ExpansionPanel>
            </Grid>
            <Grid item xs={12} md={12} lg={12}>
                <ExpansionPanel className={classes.griditem}>
                    <ExpansionPanelSummary
                        expandIcon={<ExpandMoreIcon/>}
                        aria-controls="panel1a-content"
                        id="panel1a-header"
                    >
                        <Typography>Nastavení předpovědí <i>(https://openweathermap.org/)</i></Typography>
                    </ExpansionPanelSummary>
                    <ExpansionPanelDetails>
                        <Grid container>
                            {weatherKeys.map((item) => {
                                // @ts-ignore
                                let value = values[item.key] || ""
                                return <Grid item xs={12} md={6} lg={4} key={item.key}>
                                    <Paper className={classes.properties}>
                                        <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                                            {item.label}
                                        </InputLabel>
                                        <Input
                                            id="standard-adornment-name"
                                            value={value}
                                            className={classes.input}
                                            onChange={handleChange(item.key, lengthValidator(item.max))}
                                            startAdornment={<InputAdornment
                                                position="start"><PersonPinIcon/></InputAdornment>}
                                        />
                                    </Paper>

                                </Grid>
                            })}
                        </Grid>

                    </ExpansionPanelDetails>
                </ExpansionPanel>
            </Grid>
            <Grid item xs={12} sm={12} md={12}>
                <Paper className={classes.properties} key={values.mo + ""}>
                    <FormControl component="fieldset">
                        <div className={classes.label}>
                            <FormLabel component="legend"><CompareArrowsIcon/> Režim provozu</FormLabel>
                        </div>
                        <RadioGroup aria-label="mode" name="gender1" value={values.mo}
                                    onChange={setMode} className={classes.radioBtnGroup}>
                            <FormControlLabel value={Mode.INACTIVE}
                                              control={<Radio/>}
                                              className={values.mo == Mode.INACTIVE ? classes.selectedFont : undefined}
                                              label="Neaktivní"/>
                            <FormControlLabel value={Mode.WINTER}
                                              control={<Radio/>}
                                              className={values.mo == Mode.WINTER ? classes.selectedFont : undefined}
                                              label="Ohřívání vzduchu a topení"/>
                            <FormControlLabel value={Mode.SUMMER}
                                              control={<Radio/>}
                                              className={values.mo == Mode.SUMMER ? classes.selectedFont : undefined}
                                              label={<>Noční chlazení <i>(Zařízení musí být připojené k WiFi)</i></>}/>
                            <FormControlLabel value={Mode.AUTO}
                                              control={<Radio/>}
                                              className={values.mo == Mode.AUTO ? classes.selectedFont : undefined}
                                              label={<>Automaticky <i>(Zařízení musí být připojené k WiFi, odkud si
                                                  synchronizuje čas a předpověď počasí)</i></>}/>
                        </RadioGroup>
                    </FormControl>
                </Paper>
            </Grid>
        </Grid>
        <div className={classes.winterSetting}>
            <Grid container>
                <Grid item xs={12} sm={12} md={12}>
                    <h2><AcUnitIcon/> Nastavení vyhřívacího programu</h2>
                </Grid>
                <Grid item xs={12} sm={6} md={6}>
                    <Paper className={classes.properties}>
                        <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                            <TrendingDownIcon/>
                            Sledování klesajícího trendu
                        </InputLabel>
                        <Input
                            error={maxTrendingDownInvalid}
                            id="standard-adornment-max-trending-down"
                            value={values.td}
                            className={classes.input}
                            onChange={handleChange('td', rangeValidator(0, 1800))}
                            startAdornment={<InputAdornment position="start">počet vteřin (60 až 1800)</InputAdornment>}
                        />
                        <div className={classes.helplabel}>(Ventilátor se vypne, pokud teplota v kolektoru klesá daný
                            počet
                            vteřin. Neaktivní při teplotě nad 70°C)
                        </div>
                    </Paper>
                </Grid>
                <Grid item xs={12} sm={6} md={4}>
                    <Paper className={classes.properties}>
                        <InputLabel className={classes.label}>
                            <PanToolIcon/>
                            Deaktivovat při teplotě větší než:
                        </InputLabel>
                        <Input
                            error={maxInsideTemperatureInvalid}
                            id="standard-adornment-max-inside-temp"
                            type={"number"}
                            value={values.wmit / 10}
                            className={classes.input}
                            onChange={handleChange('wmit', temperatureValidator, (val: string) => {
                                console.log(val);
                                return Math.round(Number(val) * 10)
                            })}
                            startAdornment={<InputAdornment position="start">°C</InputAdornment>}
                        />
                    </Paper>
                </Grid>
                <TurnOnOffRules header={"Pravidla pro sepnutí (max 5, od nejmenší teploty po největší):"}
                                rules={values.wor}
                                validate={winterOrderValidation}
                                onChange={handleRuleChange('wor')}/>
            </Grid></div>
        <div className={classes.summerSetting}>
            <Grid container>
                <Grid item xs={12} sm={12} md={12}>
                    <h2><WbSunnyIcon/> Nastavení chladícího programu</h2>
                </Grid>
                <Grid item xs={12} sm={6} md={4}>
                    <Paper className={classes.properties}>
                        <InputLabel className={classes.label}>
                            <PanToolIcon/>
                            Deaktivovat při teplotě menší než::
                        </InputLabel>
                        <Input
                            error={maxInsideTemperatureInvalid}
                            id="standard-adornment-min-inside-temp"
                            type={"number"}
                            value={values.smit / 10}
                            className={classes.input}
                            onChange={handleChange('smit', temperatureValidator, (val: string) => {
                                console.log(val);
                                return Math.round(Number(val) * 10)
                            })}
                            startAdornment={<InputAdornment position="start">°C</InputAdornment>}
                        />

                    </Paper>
                </Grid>
                <Grid item xs={12} sm={12} md={12}>
                    <TurnOnOffRules header={"Pravidla pro sepnutí (max 5, od nejvyšší teploty po nejmenší):"}
                                    rules={values.sor}
                                    validate={summerOrderValidation}
                                    onChange={handleRuleChange('sor')}/>
                </Grid>

            </Grid>
        </div>
        <div className={classes.autoSetting}>
            <Grid container>
                <Grid item xs={12} sm={12} md={12}>
                    <h2>Nastavení automatického programu</h2>
                </Grid>
                <Grid item xs={12} sm={12} md={12}>
                    <Paper className={classes.properties}>
                        Nastavení období vyhřívacího programu
                        <Range color="orange" start={values.aws} end={values.awe}
                               onChange={(start, end) => {
                                   setValues({...values, aws: start, awe: end})
                               }}/>
                    </Paper>
                </Grid>
                <Grid item xs={12} sm={12} md={12}>
                    <Paper className={classes.properties}>
                        Nastavení období klimatizačního programu
                        <Range color="blue" start={values.ass} end={values.ase}
                               onChange={(start, end) => {
                                   setValues({...values, ass: start, ase: end})
                               }}/>
                    </Paper>
                </Grid>
                {winterSummerOverlaps ? <div className={classes.warn}>
                    <WarningIcon/> Obě dvě období se nesmí prolínat
                </div> : ""}
            </Grid></div>
        <Button onClick={handleSave} disabled={anyInvalid} color={"secondary"} variant={"contained"}
                className={classes.btn}>Uložit</Button>
        <div className={classes.nothing}></div>
    </div>
}


const useStyles = makeStyles({
    label: {
        display: "flex",
        alignItems: "center",
    },
    helplabel: {
        fontSize: "80%",
        fontStyle: "italic"
    },
    radioBtnGroup: {
        display: "block",
    },
    griditem: {
        margin: 20,
    },
    winterSetting: {
        borderLeft: "red 2px solid",
        padding: 20,
        marginTop: 10,
    },
    autoSetting: {
        borderLeft: "orange 2px solid",
        padding: 20,
        marginTop: 10,
    },
    summerSetting: {
        borderLeft: "blue 2px solid",
        padding: 20,
        marginTop: 10,
    },
    selectedFont: {
        fontWeight: "bold",
        color: "red",
    },
    properties: {
        margin: 20,
        padding: 20
    },
    header: {},
    input: {
        margin: 20
    },
    btn: {
        margin: 20
    },
    nothing: {
        margin: 50
    },
    warn: {
        color: "orange"
    }
})