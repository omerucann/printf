#include "ft_printf.h"

static void print_space(t_info *info, long long data)
{
    if(info->spec == 's' || info->spec == 'c' || info->spec == '%')
    {
        if(info->zero && !info->align && !info->dot && !info->prec)
            return ;
        if(info->prec && info->prec < info->data_len)
            info->space_len = info->width - info->prec;
        else
            info->space_len = info->width - info->data_len;
    }
    else
    {
        if(info->data_len < info->prec)
            info->space_len = info->width - info->prec;
        else
            info->space_len = info->width - info->data_len;
        if(data < 0)
            info->space_len--;

        if(info->spec == 'p')
            info->space_len -= 2;

        if(info->zero && !info->dot && !info->prec)
            info->space_len -= info->width - info->data_len;
    }
    while(info->space_len-- > 0)
        ft_putchar(' ', info);
}

static void		print_zero(t_info *info, long long data)
{
    if(info->spec == 's' || info->spec == 'c' || info->spec == '%')
    {
        if(info->zero && !info->align)
            info->zero_len = info->width - info->data_len;
    }
    else
    {
        if(info->zero && !info->dot && !info->prec)
        {
            info->zero_len = info->width - info->data_len;
            if(data < 0)
                info->zero_len--;
        }
        else
            info->zero_len = info->prec - info->data_len;
    }
    while(info->zero_len-- > 0)
        ft_putchar('0', info);
}

static void print_diuxp(va_list ap, t_info *info)
{
    char        *base;
    long long   data;
    
    if(info->spec == 'd' || info->spec == 'i')
        data = va_arg(ap, int);
    else if(info->spec == 'u' || info->spec == 'x' || info->spec == 'X')
        data = va_arg(ap, unsigned int);
    else
        data = va_arg(ap, unsigned long);
    if(info->spec == 'd' | info->spec == 'i' || info->spec == 'u')
        base = DEC;
    else if(info->spec == 'X')
        base = HEX_UPPER;
    else
        base = HEX_LOWER;
    if(info->data_len = data < 0)
        ft_nbrlen_base(-1 * data, base, info);
    else
        ft_nbrlen_base(data, base, info);
    if(info->align == 0)
        print_space(info, data);
    if(data < 0)
        ft_putchar('-', info);
    if(info->spec == 'p')
        ft_putstr("0x", info);
    print_zero(info, data);
    if(data < 0)
        ft_putnbr_base(-1 * data, base, info);
    else
        ft_putnbr_base(data, base, info);
    if(info->align == 1)
        print_space(info, data);
    else
        info->align = 0;
}

static void print_sc(va_list ap, t_info *info)
{
    char    *data_s;
    int     data_c;

    data_s = 0;
    data_c = 0;

    if(info->spec == 's')
        data_s = va_arg(ap, char *);
    else if(info->spec == 'c')
        data_s = va_arg(ap, int);
    else
        data_c = '%';

    if(data_s == NULL)
        return (NULL);
    else
        data_s = 1;
    if(info->data_len = (info->spec == 's'))
        ft_strlen(data_s, info);
    else
        info->data_len = 1;
    if(info->align == 0)
        print_space(info, 0);
    print_zero(info, 0);
    if(info->spec == 's')
        ft_putstr(data_s, info);
    else
    ft_putchar(data_c, info);
    if(info->align == 1)
        print_space(info, 0);
}

void print_hub(va_list ap, t_info *info)
{
    if(info->spec == 'd' || info->spec == 'i' || info->spec == 'u' || info->spec == 'x' || info->spec == 'X' || info->spec == 'p')
        print_diuxp(ap, info);
    else if(info->spec == 's' || info->spec == 'c' || info->spec == '%')
        print_sc(ap, info);
}