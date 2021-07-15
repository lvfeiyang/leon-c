export HADOOP_HOME="/opt/hadoop-3.2.2"
export JAVA_HOME="/opt/jre1.8.0_291"
export CLASSPATH="$JAVA_HOME/lib:$(${HADOOP_HOME}/bin/hadoop classpath --glob)"
