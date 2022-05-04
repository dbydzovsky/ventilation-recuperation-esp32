import React from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";

export interface Props {

}

export function DisplayHelpPart(props: Props) {
    const classes = useStyles()

    return <div className={classes.root}>
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/mainscreen1.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/mainScreen2.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/alarmScreen.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/historyCo2Screen.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/tempHistory.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/disableScreen.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/party.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/manualScreen.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/inhouseScreen.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/outscreen.png
        /media/dbydzovsky/7ED6660772782131/repositories/kolektor2/images/screens/debug.png
    </div>
}

const useStyles = makeStyles((theme: Theme) => {
    return {
        root: {

        }
    }
})