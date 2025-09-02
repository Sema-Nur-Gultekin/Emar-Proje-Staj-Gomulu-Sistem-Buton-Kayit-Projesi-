using ButonBasmaBackend.Data;
using ButonBasmaBackend.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace ButonBasmaBackend.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class ButtonController : ControllerBase
    {
        private readonly ButtonDbContext _context;
        private readonly IConfiguration _configuration;

        public ButtonController(ButtonDbContext context, IConfiguration configuration)
        {
            _context = context;
            _configuration = configuration;
        }

        [HttpPost]
        public async Task<IActionResult> PostButtonPress([FromBody] ButtonRequest request)
        {

            var apiKey = _configuration["ApiKey"];
            if (Request.Headers["X-API-KEY"] != apiKey)
            {
                return Unauthorized("Geçersiz API Anahtarı.");
            }

            try
            {
                var newLog = new ButtonLog
                {
                    Tarih = DateTime.Now
                };

                _context.ButtonLogs.Add(newLog);
                await _context.SaveChangesAsync();

                return Ok(new { message = "Buton basma verisi başarıyla kaydedildi." });
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Veri tabanına yazılırken hata oluştu: {ex.Message}");
                return StatusCode(500, "Veri tabanı işlemi sırasında hata oluştu.");
            }
        }
    }
}