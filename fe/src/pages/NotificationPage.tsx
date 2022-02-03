import React from 'react'
import Snackbar from '@material-ui/core/Snackbar';
import {makeStyles} from '@material-ui/core/styles';
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import {NotificationActions, NotificationType} from "../model/notification";
import {IconButton} from "@material-ui/core";
import CloseIcon from '@material-ui/icons/Close';

export const useStyles = makeStyles((theme) => {
    return {
        success: {
            background: "green"
        },
        error: {
            background: "red"
        },
        info: {
            background: "blue"
        }
    }
})

export function NotificationWidget() {
    const notification = useSelector((state: RootState) => state.notificationState).notification;
    const dispatch = useDispatch()
    const opened = notification !== undefined;
    const handleClose = () => dispatch({type:NotificationActions.CLEAR_NOTIFICATIONS})
    const classes = useStyles();
    let clazz = classes.success;
    if (notification?.type== NotificationType.ERROR) {
        clazz = classes.error
    } else if (notification?.type== NotificationType.INFO) {
        clazz = classes.info
    }

    return <Snackbar
        anchorOrigin={{
            vertical: 'bottom',
            horizontal: 'left',
        }}
        ContentProps={{
            classes: {
                root: clazz
            }
        }}
        open={opened }
        autoHideDuration={6000}
        onClose={handleClose}
        message={notification?.type + ": " + notification?.message}
        action={
            <React.Fragment>
                <IconButton size="small" aria-label="close" color="inherit" onClick={handleClose}>
                    <CloseIcon fontSize="small" />
                </IconButton>
            </React.Fragment>
        }
    />

}