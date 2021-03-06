import React from 'react'
import FiberManualRecordIcon from '@material-ui/icons/FiberManualRecord';
import LooksOneIcon from '@material-ui/icons/LooksOne';
import Looks3Icon from '@material-ui/icons/Looks3';
import LooksTwo from '@material-ui/icons/LooksTwo';
import TimerIcon from '@material-ui/icons/Timer';
import Grid from "@material-ui/core/Grid";
import {makeStyles} from "@material-ui/styles";
import {Theme} from "@material-ui/core/styles";
import {InstallationPage} from "./InstallationPage";
import ExpansionPanelSummary from "@material-ui/core/ExpansionPanelSummary";
import ExpandMoreIcon from "@material-ui/icons/ExpandMore";
import Typography from "@material-ui/core/Typography";
import ExpansionPanelDetails from "@material-ui/core/ExpansionPanelDetails";
import {Input, InputAdornment, InputLabel, Paper} from "@material-ui/core";
import PersonPinIcon from "@material-ui/icons/PersonPin";
import ExpansionPanel from "@material-ui/core/ExpansionPanel";
import WarningIcon from "@material-ui/icons/Warning";
import {DisplayHelpPart} from "../components/DisplayHelpPart";
import GitHubIcon from '@material-ui/icons/GitHub';
const githubLink = "https://github.com/dbydzovsky/ventilation-recuperation-esp32";
enum SignalDiode {
    Red = "red",
    Green = "green",
    Orange = "orange",
    None = "none"
}

const Red = SignalDiode.Red;
const Green = SignalDiode.Green;
const Orange = SignalDiode.Orange;
const None = SignalDiode.None;

function DiodeSignalization(props: { data: (SignalDiode)[] }) {
    if (props.data.length != 12) {
        throw new Error("Must be 12 in size.")
    }
    return <>
        {props.data.map(signal => {
            let color = "lightgrey";
            if (signal == SignalDiode.Red) {
                color = "red";
            } else if (signal == SignalDiode.Green) {
                color = "green"
            } else if (signal == SignalDiode.Orange) {
                color = "orange"
            }
            return <FiberManualRecordIcon htmlColor={color}/>
        })}
    </>
}

const TrialSignal = [Green, None, Orange, None, Green, None, Orange, None, Green, None, Orange, None];
const WiFiConnectingSignal = [Green, Orange, Red, Green, Orange, Red, Green, Orange, Red, Green, Orange, Red];
const WinterDeviceOperationalWifiOnSignal = [None, None, None, None, None, None, None, None, None, Orange, None, Orange];
const WinterDeviceOperationalWifiOffSignal = [None, None, None, None, None, None, None, None, None, None, None, Orange];
const SummerDeviceOperationalWifiOnSignal = [None, None, None, None, None, None, None, None, None, Green, None, Green];
const SummerDeviceOperationalWifiOffSignal = [None, None, None, None, None, None, None, None, None, None, None, Green];
const DeviceDisabledWifiOnSignal = [None, None, None, None, None, None, None, None, None, Red, None, Red];
const DeviceDisabledWifiOffSignal = [None, None, None, None, None, None, None, None, None, None, None, Red];
const WarmingUpSignal = [Orange, Orange, None, Orange, None, Orange, Orange, None, Orange, None, Orange, Orange];
const NotValidSensorValues = [Red, None, Red, None, Red, None, Red, None, Red, None, Red, None];
const DisabledDueToConditions = [None, None, None, None, None, None, None, None, Green, Green, Orange, Orange];
const CriticalSignal = [Red, Red, Red, Red, None, None, None, None, Red, Red, Red, Red];

export function Row(props: { label: any, value: any }) {
    const classes = useStyles();
    return <Grid item xs={12}>
        <Grid container className={classes.row}>
            <Grid item className={classes.flex} xs={1}>
                {props.label}
            </Grid>
            <Grid item className={classes.flex} xs>
                {props.value}
            </Grid>
        </Grid>
    </Grid>
}

export function HelpPage() {
    const classes = useStyles();
    return <div className={classes.root}>
        <div className={classes.center}>
            <h1>N??pov??da - ovl??d??n?? rekuperace</h1>
        </div>
        {/*<h2 className={classes.header}>Display</h2>*/}
        {/*<h2 className={classes.header}>V??znam sv??teln?? signalizace</h2>*/}
        {/*<Grid container className={classes.group}>*/}
        {/*    <Row label={<DiodeSignalization data={WiFiConnectingSignal}/>}*/}
        {/*         value="Za????zen?? se p??ipojuje k WiFi nebo se nach??z?? v m??du Access Point, aby mohlo b??t nakonfigurov??no."/>*/}
        {/*    <Row label={<DiodeSignalization data={WarmingUpSignal}/>}*/}
        {/*         value="Za????zen?? bylo pr??v?? zapnuto a zah????vaj?? se sensory"/>*/}
        {/*    <Row label={<DiodeSignalization data={WinterDeviceOperationalWifiOnSignal}/>}*/}
        {/*         value="Za????zen?? je aktivn?? (a p??ipojen?? k WiFi), a to v m??du vyh????v??n??"/>*/}
        {/*    <Row label={<DiodeSignalization data={WinterDeviceOperationalWifiOffSignal}/>}*/}
        {/*         value="Za????zen?? je aktivn?? (ale odpojen?? od WiFi), a to v m??du vyh????v??n??"/>*/}
        {/*    <Row label={<DiodeSignalization data={SummerDeviceOperationalWifiOnSignal}/>}*/}
        {/*         value="Za????zen?? je aktivn?? (a p??ipojen?? k WiFi), a to v m??du chlazen??"/>*/}
        {/*    <Row label={<DiodeSignalization data={SummerDeviceOperationalWifiOffSignal}/>}*/}
        {/*         value="Za????zen?? je aktivn?? (ale odpojen?? od WiFi), a to v m??du chlazen??"/>*/}
        {/*    <Row label={<DiodeSignalization data={DeviceDisabledWifiOnSignal}/>}*/}
        {/*         value="Za????zen?? je neaktivn?? (ale p??ipojen?? k WiFi) a nedojde k automatick??mu spu??t??n??"/>*/}
        {/*    <Row label={<DiodeSignalization data={DeviceDisabledWifiOffSignal}/>}*/}
        {/*         value="Za????zen?? je neaktivn?? (a odpojen?? od WiFi) a nedojde k automatick??mu spu??t??n??"/>*/}
        {/*    <Row label={<DiodeSignalization data={TrialSignal}/>}*/}
        {/*         value="Za????zen?? je do??asn?? ovl??d??no manu??ln?? z webov??ho rozhran??"/>*/}
        {/*    <Row label={<DiodeSignalization data={DisabledDueToConditions}/>}*/}
        {/*         value="Ventil??tor je vypnut?? kv??li nastaven??m podm??nk??m."/>*/}
        {/*    <Row label={<DiodeSignalization data={CriticalSignal}/>}*/}
        {/*         value="Chybn?? konfigurace. Vytvo??te a ulo??te novou konfiguraci ve webov??m rozhran??."/>*/}
        {/*    <Row label={<DiodeSignalization data={NotValidSensorValues}/>}*/}
        {/*         value="Nelze p??e????st hodnoty ze sensor??. Zkontrolujte webov?? rozhran?? a hodnoty ??idel."/>*/}
        {/*</Grid>*/}
        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Display</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                <div>
                    <p>Display zobrazuje aktu??ln?? hodnoty ??idel. Umo????uje tak?? zapnut?? tzv. "p??rty" m??du, pop??. ventilaci a rekuperaci na chv??li deaktivaci.
                    Retrospektivn?? tak?? ukazuje hodnoty CO<sub>2</sub> a vn??j???? teploty.</p>
                    <DisplayHelpPart/>
                </div>
            </ExpansionPanelDetails>
        </ExpansionPanel>
        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Ovl??d??n?? dotykov??ho tla????tka</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>

                <Grid container className={classes.group}>
                    <Row label={<><LooksOneIcon/> kliknut??</>}
                         value={<>P??i kliknut?? se p??epne obrazovka na n??sleduj??c??.</>}/>
                    <Row label={<><LooksTwo/> 2kliknut??</>}
                         value={<>P??i kliknut?? se p??epne obrazovka na p??edchoz??.</>}/>
                    <Row label={<><TimerIcon/> Podr??et 4s</>}
                         value={<>
                             P??i potvr??en?? tla????tka se aktivuje akce, kter?? p????slu???? dan?? obrazovce.
                         </>}/>
                    <Row label={<><TimerIcon/> Podr??et 10s</>}
                         value={<>
                             P??i podr??en?? tla????ka na obrazovce bez aktivn??ho tla????tka se po 10 vte??in??ch za????zen?? restartuje.
                         </>}/>
                </Grid>
            </ExpansionPanelDetails>
        </ExpansionPanel>

        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>??idlo CO<sub>2</sub></Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                <p>
                    Za????zen?? obsahuje ??idlo s n??zvem MH-Z19B, jeho?? rozp??t?? hodnot je od
                    0 a?? 5000 PPM (Parts Per Million).
                    Toto ??idlo je nastaven?? pro automatickou kalibraci, kter?? se prov??d?? ka??d??ch 24
                    hodin od zapnut?? za????zen??. Nejmen???? nam????en?? hodnota za posledn??ch 24 hodin se
                    nastav?? jako reprezentativn??ch 400 PPM.

                    Toto nastaven?? je vhodn?? do vnit??n??ch prostor s ob??asn??m v??tr??n??m.
                </p>
            </ExpansionPanelDetails>
        </ExpansionPanel>

        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Zm??na WiFi p??ipojen??</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                <div>
                    <p>Pro zm??nu WiFi s??t?? (nebo jej??ho hesla) je pot??eba za????zen?? restartovat, co?? lze prov??zt podr??en??m dotykov??ho tla????tka po dobu 10 vte??in.</p>
                    <p>Za????zen?? se po restartu nepoda???? p??ipojit k s??ti a proto p??ejde do m??du Access Point, kdy je mo??n?? ho op??t konfigurovat.</p>
                    <p>
                        <ul>
                            <li>Default SSID: SolarAirCollectorV2</li>
                            <li>Default heslo: 123456789</li>
                        </ul>
                    </p>
                </div>
            </ExpansionPanelDetails>
        </ExpansionPanel>

        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>S??mantika programov??ch k??d??</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                <div className="pageBreak">
                    <p>Do monitoringu se pos??l?? tak?? programov?? k??d, tj. aktu??ln?? ????slo programu. Ka??d?? ????slo m?? konkr??tn?? v??znam.</p>
                    <p>Toto ????slo je n??pomocn??, pokud se zp??tn?? hodnot?? p??i??ina chov??n?? syst??mu.</p>
                    <br/>
                    <Grid container className={classes.group}>
                        <Row label={0} value={"Nenastaveno"}/>
                        <Row label={5.1} value={"Nevalidn?? hodnota z venkovn??ho ??idla teploty"}/>
                        <Row label={5.2} value={"Nevalidn?? hodnota z vnit??n??ho ??idla teploty"}/>
                        <Row label={5.3} value={"Nevalidn?? hodnota z venkovn??ho ??idla vlhkosti"}/>
                        <Row label={7.0} value={"Inicializa??n?? program"}/>
                        <Row label={8.0} value={"Chybn?? konfigurace"}/>
                        <Row label={9.0} value={"Program je vypnut??"}/>
                        <Row label={10.0} value={"Manu??ln?? program"}/>
                        <Row label={11.0} value={"Chlad??c?? program"}/>
                        <Row label={11.1} value={"Chlad??c?? program - c??lov?? teploty bylo dos??hnuto"}/>
                        <Row label={11.3} value={"Chlad??c?? program - rosn?? bod p??ekra??uje vnit??n?? teplotu"}/>
                        <Row label={12.0} value={"Vyh????vac?? program"}/>
                        <Row label={12.1} value={"Vyh????vac?? program - c??lov?? teploty bylo dos??hnuto"}/>
                        <Row label={12.2} value={"Vyh????vac?? program - teplota v kolektoru pomalu kles??"}/>
                        <Row label={12.3} value={"Vyh????vac?? program - rosn?? bod p??ekra??uje vnit??n?? teplotu"}/>
                    </Grid>
                </div>
            </ExpansionPanelDetails>
        </ExpansionPanel>

        {/*    <div className={classes.center}>*/}
        {/*    <h1>Sch??ma</h1>*/}
        {/*    <GitHubIcon/> V??ce o projektu na <a href={"https://gitlab.com/d.bydzovsky/solar-air-collector-esp8266"}>https://gitlab.com/d.bydzovsky/solar-air-collector-esp8266</a><br/>*/}
        {/*</div>*/}

        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Instala??n?? n??vod a sch??ma</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                <InstallationPage/>
            </ExpansionPanelDetails>
        </ExpansionPanel>
        <ExpansionPanel>
            <ExpansionPanelSummary
                expandIcon={<ExpandMoreIcon/>}
                aria-controls="panel1a-content"
                id="panel1a-header"
            >
                <Typography>Zdrojov?? k??d</Typography>
            </ExpansionPanelSummary>
            <ExpansionPanelDetails>
                <p><GitHubIcon/>Zdrojov?? k??d je ve??ejn??. <a href={githubLink} target={"_blank"}>{githubLink}</a></p>
            </ExpansionPanelDetails>
        </ExpansionPanel>
    </div>
}


const useStyles = makeStyles((theme: Theme) => ({
    root: {
        margin: 20
    },
    row: {
    },
    row2: {
        marginTop: 20,
        border: "1px dashed black"
    },
    group: {
    },
    flex: {
        display: "flex",
        alignItems: "center",
        justifyContent: "left",
    },
    header: {
        marginTop: 20,
        fontSize: 15
    },
    center: {
        textAlign: "center"
    }
}));