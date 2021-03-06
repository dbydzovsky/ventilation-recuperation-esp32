
import React from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {Images} from "../images/Images";
import Grid from "@material-ui/core/Grid";
import WarningIcon from '@material-ui/icons/Warning';
export interface Props {

}

export function InstallationPage(props: Props) {
    const classes = useStyles();
    let properties = [
        {property: "Max. výkon"},
        {property: "StandBy spotřeba bez periférií"},
        {property: "StandBy spotřeba"},
        {property: "Spotřeba 10% výkon"},
        {property: "Spotřeba 25% výkon"},
        {property: "Spotřeba 50% výkon"},
        {property: "Spotřeba 75% výkon"},
        {property: "Spotřeba 100% výkon"},
        {property: "Datum montáže"},
    ]
    return <div className={classes.root}>
        <Grid container>
            <p><WarningIcon htmlColor={"orange"}/>
                Před jakoukoli montáží je třeba zařízení odpojit od přivodu elektrické energie.
                Při jakékoliv montáží je třeba pečlivě sledovat značení na desce.
                Nejsou povolené žádné vlastní úpravy, které by mohli ohrozit funkčnost zařízení.
            </p>
            <p>
                Je doporučeno projít si pokročilé nastavení
                (k dispozici v uživatelském rozhraní prostřednictvím internetového prohlížeče),
                kde je možné zařízení dále konfigurovat dle požadavků prostředí.
            </p>
            <p>
                Pro sledování případných problémů je určena stránka "Firmware", kde
                lze sledovat ladící zprávy. Tyto ladící zprávy zmizí při restartu zařízení.
                Pokud nastane problém, bude zde k dispozici chybová ladící zpráva, kterou je vhodné
                zarchivovat.
            </p>
            <p>
                Ventilátor lze použít 12V nebo 24V (Tj. celé zařízení je nutné zapojit na 12V).
                Je důležité nepřekročit hodnotu proudu 2A, neboť
                PCB je stavěná právě na tento maximální proud.
                Tj. při použítí větráku 12V je povolený maximální výkon 24W.
                Je nutné odběr nepřekročit, jinak hrozí
                poškození či zníčení zdroje (tj. součet maximální spotřeby všech komponent nesmí
                překročit povolený odběr).

                <b>NEBLOKUJTE přívod vzduchu ventilátoru.</b> V závislosti na odporu vzduchu
                je možné, že se zvýší odběr ventilátoru a překročí tak povolený odběr, což
                může poškodit zařízení.

                <b>Při použití 12V zdrojového napětí musí být maximální výkon 24W.</b>
                <b>Při použití 24V zdrojového napětí musí být maximální výkon 48W.</b>
            </p>
            <p>
                Komunikační linka PWM do větráku má 3.3V a je na přímo projená s řídicí jednotkou.
                Její maximální odběr je tak dle specifikace ESP32 20-40 mA.
                Při instalaci lze nastavit frekvenci pwm, a to 20MHz nebo 25MHz.
                Resolution pwm je nastavené na 8.
            </p>
            <p>
                Komunikační linka pro čtení rychlosti otáček (Signal Input) je propojené galvanicky, nepřímo, přes
                optočlen. Komunikační linka pro čtení otáček je připojena ke zdroji napětí (tj. 12V nebo 24V).
                Větrák tak musí být schopen vyčítat otáčky tímto vyšším napětím.
            </p>
            <p>
                Celková délka komunikačních kabelů by měla být co nejkratší, zároveň kratší než 5m.
                V případě příliš dlouhého kabelu může dojít k poruchám čtení ze sensorů a
                často nefunkčnímu displayi. Jako komunikační protokol se použítá I2C.
            </p>
            <Grid item xs={12} className={"showOnPrint"}>
                <table>
                    {properties.map( obj => {
                        return <tr key={obj.property}>
                            <td>{obj.property}</td>
                            <td style={{width: 200}}></td>
                        </tr>
                    })}
                </table>
            </Grid>
            <Grid item xs={12} className={"hideOnPrint"}>
                <h2>Sensor</h2>
                <img className={classes.image} width={300} src={Images.sht20.src}/>
            </Grid>
            <Grid item xs={12} className="hideOnPrint">
                <h2>Pinout Wemos D1 ESP32 mini</h2>
                <img  className={classes.image} width={700} src={Images.pinout.src} />
            </Grid>
            <Grid item xs={12}>
                <h2>Deska</h2>
            </Grid>
            <Grid item xs={12} className="hideOnPrint">
                <img  className={classes.image} width={700} src={Images.boardPic.src}/>
            </Grid>
            <Grid item xs={12} className="hideOnPrint">
                <img className={classes.image} width={700} src={Images.board.src}/>
            </Grid>
            <Grid item xs={12} className="hideOnPrint">
                <img className={classes.image} width={700} src={Images.board2.src}/>
            </Grid>
            <Grid item xs={12}>
                <h2>Návrh</h2>
            </Grid>
            <Grid item xs={12} className="hideOnPrint">
                <img  className={classes.image} width={700} src={Images.design.src}/>
            </Grid>
            <Grid item xs={12} className="hideOnPrint">
                <img className={classes.image} width={700} src={Images.design2.src}/>
            </Grid>
            <Grid item xs={12} className="hideOnPrint">
                <img className={classes.image} width={700} src={Images.design3.src}/>
            </Grid>
            <Grid item xs={12}>
                <h1>RJ45 kabely</h1>
                <p>RJ45 kabely musí používat standard RJ45-A</p>
            </Grid>
            <Grid item xs={12} md={6} className={"hideOnPrint"}>
                <img className={classes.image} width={300} src={Images.rj45.src}/>
            </Grid>
            <Grid item xs={12} md={6}  className={"hideOnPrint"}>
                <img className={classes.image} width={300} src={Images.conector.src}/>
            </Grid>
            <Grid item xs={12} md={6}  className={"hideOnPrint"}>
                <Wires title={"Display Konektor"} desc={["SCL", "GND", "27 (reserved)", "32 (reserved)", "25 (reserved)", "5V", "3.3V", "SDA"]}/>
            </Grid>
            <Grid item xs={12} md={6}  className={"hideOnPrint"}>
                <Wires title={"Sensor Konektor"} desc={["SCL", "GND", "36 - RX - CO2", "5 - TX - CO2", "26 - Button", "5V", "3.3V", "SDA"]}/>
            </Grid>
        </Grid>
    </div>
}

export function Wires(props:{title: string, desc: string[]}) {
    return <>
        <h2>{props.title}</h2>
        <table>
        <WireRow stripped color={"green"}>{props.desc[0]}</WireRow>
        <WireRow color={"green"}>{props.desc[1]}</WireRow>
        <WireRow stripped color={"orange"}>{props.desc[2]}</WireRow>
        <WireRow color={"blue"}>{props.desc[3]}</WireRow>
        <WireRow stripped color={"blue"}>{props.desc[4]}</WireRow>
        <WireRow color={"orange"}>{props.desc[5]}</WireRow>
        <WireRow stripped color={"brown"}>{props.desc[6]}</WireRow>
        <WireRow color={"brown"}>{props.desc[7]}</WireRow>
    </table></>
}

export function WireRow(props: {children: any, stripped?: boolean, color: "brown" |"green" |"orange"|"blue"}) {
    const classes = useStyles()
    const style = {background: props.color}
    return <tr>
        <td className={classes.row}>
            <div className={classes.squeare} style={style}> </div>
            <div className={classes.squeare} style={props.stripped ? {} : style}> </div>
            <div className={classes.squeare} style={{background: props.color}}> </div>
            <div className={classes.squeare} style={props.stripped ? {} : style}> </div>
            <div className={classes.squeare} style={{background: props.color}}> </div>
            <div className={classes.squeare} style={props.stripped ? {} : style}> </div>
            <div className={classes.squeare} style={{background: props.color}}> </div>
        </td>
        <td>{props.children}</td>
    </tr>
}

const useStyles = makeStyles((theme: Theme) => {
    return {
        root: {

        },
        image:{
            maxWidth: "100%"
        },
        squeare: {
            width: 20,
            height: 20
        },
        row: {
            border: "1px solid black",
            display: "inline-flex"
        }
    }
})