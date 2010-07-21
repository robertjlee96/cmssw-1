DROP TABLE SM_SUMMARY;

CREATE TABLE SM_SUMMARY (
   RUNNUMBER          NUMBER(10)      --CREATE (NEEDED BY ALL)
  ,SETUPLABEL         VARCHAR2(100)   --CREATE
  ,APP_VERSION        VARCHAR2(100)   --CREATE
  ,S_LUMISECTION      NUMBER(10)      --CREATE
  ,S_FILESIZE         NUMBER(20)      --INJECTED
  ,S_FILESIZE2D       NUMBER(20)      --INJECTED
  ,S_FILESIZE2T0      NUMBER(20)      --COPIED (QUERY)
  ,S_NEVENTS          NUMBER(20)      --INJECTED 
  ,S_CREATED          NUMBER(10)      --CREATE
  ,S_INJECTED         NUMBER(10)      --INJECTED
  ,S_NEW              NUMBER(10)      --NEW
  ,S_COPIED           NUMBER(10)      --COPIED
  ,S_CHECKED          NUMBER(10)      --CHECKED
  ,S_INSERTED         NUMBER(10)      --INSERTED
  ,S_REPACKED         NUMBER(10)      --REPACKED
  ,S_DELETED          NUMBER(10)      --DELETED
  ,M_INSTANCE         NUMBER(5)      --CREATED
  ,START_WRITE_TIME   TIMESTAMP       --CREATED
  ,STOP_WRITE_TIME    TIMESTAMP       --INJECTED
  ,START_TRANS_TIME   TIMESTAMP       --NEW
  ,STOP_TRANS_TIME    TIMESTAMP       --COPIED
  ,START_REPACK_TIME  TIMESTAMP       --CHECKED
  ,STOP_REPACK_TIME   TIMESTAMP       --REPACKED
  ,HLTKEY             VARCHAR2(1000)  --INJECTED
  ,LAST_UPDATE_TIME   TIMESTAMP       --
  ,STREAM             VARCHAR2(20)    NOT NULL
  ,N_INSTANCE         NUMBER(5)
  ,CONSTRAINT PK_RN PRIMARY KEY (RUNNUMBER)
);
