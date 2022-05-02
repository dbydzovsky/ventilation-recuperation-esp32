import React from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {Container} from "@material-ui/core";
import WarningIcon from "@material-ui/icons/Warning";

export interface Props {

}

export function UpdatePage(props: Props) {
    const classes = useStyles()

    return <div className={classes.root}>
        <Container>
            <h1>Update</h1>
            <p><WarningIcon htmlColor={"orange"}/>
                <b>Z bezpečnostních důvodů</b> je přístupový bod vypnutý.
            </p>
        </Container>
    </div>
}

const useStyles = makeStyles((theme: Theme) => {
    return {
        root: {

        }
    }
})