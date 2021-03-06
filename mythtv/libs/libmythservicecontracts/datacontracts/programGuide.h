//////////////////////////////////////////////////////////////////////////////
// Program Name: programGuide.h
// Created     : Jan. 15, 2010
//
// Purpose     : 
//                                                                            
// Copyright (c) 2010 David Blain <dblain@mythtv.org>
//                                          
// Licensed under the GPL v2 or later, see COPYING for details
//
//////////////////////////////////////////////////////////////////////////////

#ifndef PROGRAMGUIDE_H_
#define PROGRAMGUIDE_H_

#include <QDateTime>
#include <QString>

#include "serviceexp.h" 
#include "datacontracthelper.h"

#include "programAndChannel.h"

namespace DTC
{

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// -=>NOTE: DESIGNABLE - is used to indicate if should be Serialized 
//                       (can specify a propery to support runtime logic)
//                       
//
//          Q_CLASSINFO( "defaultProp", "<propname>" ) -
//                       is used to indicate the default property 
//                       (used for node text in XML)
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class SERVICE_PUBLIC ProgramGuide : public QObject
{
    Q_OBJECT
    Q_CLASSINFO( "version", "1.0" );

    // We need to know the type that will ultimately be contained in 
    // any QVariantList or QVariantMap.  We do his by specifying
    // A Q_CLASSINFO entry with "<PropName>_type" as the key
    // and the type name as the value

    Q_CLASSINFO( "Channels", "type=DTC::ChannelInfo");


    Q_PROPERTY( QDateTime     StartTime      READ StartTime      WRITE setStartTime      )
    Q_PROPERTY( QDateTime     EndTime        READ EndTime        WRITE setEndTime        )
    Q_PROPERTY( int           StartChanId    READ StartChanId    WRITE setStartChanId    )
    Q_PROPERTY( int           EndChanId      READ EndChanId      WRITE setEndChanId      )
    Q_PROPERTY( int           NumOfChannels  READ NumOfChannels  WRITE setNumOfChannels  )
                                                                         
    Q_PROPERTY( bool          Details        READ Details        WRITE setDetails        )
    Q_PROPERTY( int           Count          READ Count          WRITE setCount          )
    Q_PROPERTY( QDateTime     AsOf           READ AsOf           WRITE setAsOf           )
    Q_PROPERTY( QString       Version        READ Version        WRITE setVersion        )
    Q_PROPERTY( QString       ProtoVer       READ ProtoVer       WRITE setProtoVer       )

    Q_PROPERTY( QVariantList Channels READ Channels DESIGNABLE true )

    PROPERTYIMP       ( QDateTime   , StartTime     )
    PROPERTYIMP       ( QDateTime   , EndTime       )
    PROPERTYIMP       ( int         , StartChanId   )
    PROPERTYIMP       ( int         , EndChanId     )
    PROPERTYIMP       ( int         , NumOfChannels )
                                                      
    PROPERTYIMP       ( bool        , Details       )
    PROPERTYIMP       ( int         , Count         )
    PROPERTYIMP       ( QDateTime   , AsOf          )
    PROPERTYIMP       ( QString     , Version       )
    PROPERTYIMP       ( QString     , ProtoVer      )

    PROPERTYIMP_RO_REF( QVariantList, Channels      )

    public:

        static void InitializeCustomTypes()
        {
            qRegisterMetaType< ProgramGuide  >();
            qRegisterMetaType< ProgramGuide* >();

            ChannelInfo::InitializeCustomTypes();
        }

    public:

        ProgramGuide(QObject *parent = 0) 
            : QObject           ( parent ),
              m_StartChanId     ( 0      ),     
              m_EndChanId       ( 0      ),
              m_NumOfChannels   ( 0      ),
              m_Details         ( false  ),
              m_Count           ( 0      )  
        {
        }
        
        ProgramGuide( const ProgramGuide &src )
        {
            Copy( src );
        }

        void Copy( const ProgramGuide &src )
        {                                       
            m_StartTime    = src.m_StartTime    ;
            m_EndTime      = src.m_EndTime      ;
            m_StartChanId  = src.m_StartChanId  ;
            m_EndChanId    = src.m_EndChanId    ;
            m_NumOfChannels= src.m_NumOfChannels;
            m_Details      = src.m_Details      ;
            m_Count        = src.m_Count        ;
            m_AsOf         = src.m_AsOf         ;
            m_Version      = src.m_Version      ;
            m_ProtoVer     = src.m_ProtoVer     ;
        
            CopyListContents< ChannelInfo >( this, m_Channels, src.m_Channels );
        }

        ChannelInfo *AddNewChannel()
        {
            // We must make sure the object added to the QVariantList has
            // a parent of 'this'

            ChannelInfo *pObject = new ChannelInfo( this );
            Channels().append( QVariant::fromValue<QObject *>( pObject ));

            return pObject;
        }

};

} // namespace DTC

Q_DECLARE_METATYPE( DTC::ProgramGuide  )
Q_DECLARE_METATYPE( DTC::ProgramGuide* )

#endif
