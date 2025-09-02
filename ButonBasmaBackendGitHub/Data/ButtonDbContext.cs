using ButonBasmaBackend.Models;
using Microsoft.EntityFrameworkCore;

namespace ButonBasmaBackend.Data
{
    public class ButtonDbContext : DbContext
    {
        public ButtonDbContext(DbContextOptions<ButtonDbContext> options)
            : base(options)
        {
        }

        public DbSet<ButtonLog> ButtonLogs { get; set; }
    }
}