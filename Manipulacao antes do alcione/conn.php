<?php

   class Connection {
      var $host = '192.168.0.99';
      var $port = '5432';
      var $dbname = 'e-cidade';
      var $user = 'postgres';
      var $password = '';
      var $query = '';
      var $pdo = null;
      
      function __construct() {
         $this->pdo = new PDO("pgsql:host=$this->host;
                 port=$this->port;
                 dbname=$this->dbname;
                 user=$this->user;
                 password=$this->password");
         //seta para imprimir e parar script quando PDO lançar erro
         $this->pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION); 
      }
      
      function query($str) {
         $this->query = $str;
      }
      
      private function prepare() {
         return $this->pdo->prepare($this->query, array(PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY));
      }
      
      
      function fetch($arr=null) {
         if($arr == null) $arr = array();
         $sth = $this->prepare();
         $sth->execute($arr);
         return $sth->fetchAll();
      }
      
      $sth = execute($a);
      
      
      function execute($arr=null) {
         if($arr == null) $arr = array();
         $sth = $this->prepare();
         return $sth->execute($arr);
      }
      
      function query_fetch($sql, $arr){
          $this->query($sql);
          return $this->fetch($arr);
      }
      
   }

   $conn = new Connection();


?>
