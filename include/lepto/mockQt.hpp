#ifndef LEPTO_MOCK_QT_HPP
#define LEPTO_MOCK_QT_HPP
/**---------------------------------------------------------------------------
 *
 * @file    mockQt.h
 * @brief   Classes and makros to get trivial qt applications compile qithout qt
 *
 * @date      20150711
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Defines --------------------------------------------------------------*/


#define QString CString
#define QList CList
#define QQueue CRing
#define QByteArray CByteArray
#define QSharedPointer CSharedPointer

#define Q_OBJECT
#define NO_QT
#define signals public
#define emit(A)   A.emitSignal
// #define throw  void
#define slots

#define qWarning     lWarning
#define qFatal       lFatal
#define qDebug       lDebug
#define qInfo        lInfo
#define qCritical    lCritical


/*--- Declarations ---------------------------------------------------------*/


class QObject
{

};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_MOCK_QT_HPP
