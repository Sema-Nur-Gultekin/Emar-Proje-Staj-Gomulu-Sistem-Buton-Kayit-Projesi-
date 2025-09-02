using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ButonBasmaBackend.Models
{
    [Table("TBL_BUTTON")]
    public class ButtonLog
    {
        [Key]
        [Column("ID")]
        public int Id { get; set; }

        [Column("TARIH")]
        public DateTime Tarih { get; set; }
    }
}