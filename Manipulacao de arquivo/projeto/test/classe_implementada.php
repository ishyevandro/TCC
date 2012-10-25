<?php

class Periodos {
   
   private $conn;
   public $periodos;
   
   public function __construct($conn,$turma) {
      $this->conn = $conn;
      $this->periodos = $this->consulta_periodos($turma);
   }
   
   public static function periodocalendario($conn,$id) {
      $sql = "SELECT periodocalendario.ed53_d_inicio as dt_inicial, periodocalendario.ed53_d_fim as dt_final, periodocalendario.ed53_i_diasletivos as diasletivos, procavaliacao.ed41_i_codigo as procavaliacao
FROM escola.periodocalendario
JOIN escola.procavaliacao ON periodocalendario.ed53_i_periodoavaliacao = procavaliacao.ed41_i_periodoavaliacao
WHERE periodocalendario.ed53_i_codigo = :id";

      $conn->query($sql);
      $arr = $conn->fetch(array(':id' => $id));
      
      return $arr[0];
   }
   
   private function consulta_periodos($turma) {
      $sql = "SELECT periodoavaliacao.ed09_c_descr AS periodo, periodoavaliacao.ed09_i_codigo AS cod_periodo, procavaliacao.ed41_i_codigo AS procavaliacao,
periodocalendario.ed53_d_inicio as dt_inicial, periodocalendario.ed53_d_fim as dt_final, periodocalendario.ed53_i_diasletivos as diasletivos, periodocalendario.ed53_i_codigo as periodocalendario
FROM escola.turma
JOIN escola.turmaserieregimemat ON turma.ed57_i_codigo = turmaserieregimemat.ed220_i_turma
JOIN escola.calendario ON turma.ed57_i_calendario = calendario.ed52_i_codigo
JOIN escola.periodocalendario ON periodocalendario.ed53_i_calendario = calendario.ed52_i_codigo
JOIN escola.periodoavaliacao ON periodocalendario.ed53_i_periodoavaliacao = periodoavaliacao.ed09_i_codigo
JOIN escola.procavaliacao ON periodoavaliacao.ed09_i_codigo = procavaliacao.ed41_i_periodoavaliacao AND turmaserieregimemat.ed220_i_procedimento = procavaliacao.ed41_i_procedimento
WHERE turma.ed57_i_codigo = :tur
ORDER BY periodoavaliacao.ed09_i_sequencia";

      $this->conn->query($sql);
      $arr = $this->conn->fetch(array(':tur' => $turma));
      

      return $arr;
   }
   
   public static function get_data_bloqueio($conn,$turma,$regencia) {
    $sql = "SELECT periodoavaliacao.ed09_c_descr AS periodo, periodoavaliacao.ed09_i_codigo AS cod_periodo, procavaliacao.ed41_i_codigo AS procavaliacao,
periodocalendario.ed53_d_inicio as dt_inicial, periodocalendario.ed53_d_fim as dt_final, periodocalendario.ed53_i_diasletivos as diasletivos, periodocalendario.ed53_i_codigo as periodocalendario,
e2_diariobloqueio.data_bloqueio
FROM escola.turma
JOIN escola.turmaserieregimemat ON turma.ed57_i_codigo = turmaserieregimemat.ed220_i_turma
JOIN escola.calendario ON turma.ed57_i_calendario = calendario.ed52_i_codigo
JOIN escola.periodocalendario ON periodocalendario.ed53_i_calendario = calendario.ed52_i_codigo
JOIN escola.periodoavaliacao ON periodocalendario.ed53_i_periodoavaliacao = periodoavaliacao.ed09_i_codigo
JOIN escola.procavaliacao ON periodoavaliacao.ed09_i_codigo = procavaliacao.ed41_i_periodoavaliacao AND turmaserieregimemat.ed220_i_procedimento = procavaliacao.ed41_i_procedimento
left JOIN escola.e2_diariobloqueio ON e2_diariobloqueio.id_procavaliacao = procavaliacao.ed41_i_codigo
WHERE turma.ed57_i_codigo = :tur AND (e2_diariobloqueio.id_regencia = :reg OR e2_diariobloqueio IS NULL)
ORDER BY periodoavaliacao.ed09_i_sequencia";

      $conn->query($sql);
      $arr = $conn->fetch(array(':tur' => $turma, 'reg' => $regencia));
      
      return $arr;
   }
   
   public static function atualizaAulasDadas($conn,$regencia,$procavaliacao,$calendario) {
       $conn->query("SELECT ed53_i_diasletivos FROM escola.periodocalendario
JOIN escola.procavaliacao ON procavaliacao.ed41_i_periodoavaliacao = periodocalendario.ed53_i_periodoavaliacao
WHERE procavaliacao.ed41_i_codigo = :proc AND periodocalendario.ed53_i_calendario = :cal");
       $dl = $conn->fetch(array('proc' => $procavaliacao, 'cal' => $calendario));
       $dl = $dl[0][0];
       
       $conn->query("UPDATE escola.regenciaperiodo SET ed78_i_aulasdadas = :dl
WHERE ed78_i_regencia = :reg AND ed78_i_procavaliacao = :proc");
       
       return $conn->execute(array('proc' => $procavaliacao, 'reg' => $regencia, 'dl' => $dl));
   }
   
   public function get_por_id($id) {
      if($this->periodos == null) {
         return null;
      }else {
         $periodo = null;
         foreach($this->periodos as $p) {
            if($p['cod_periodo'] == $id) {
               $periodo = $p;
               break;
            }
         }
         return $periodo;
      }
   }
   
}


?>
