/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anarama <anarama@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:44:46 by vvobis            #+#    #+#             */
/*   Updated: 2024/07/30 11:45:03 by victor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_clean	*lst_node_new(void *content, void (*del)(void *))
{
	t_clean	*new;

	if (!content || !del)
		return (NULL);
	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->content = content;
	new->clean = del;
	new->next = NULL;
	return (new);
}

void	lst_node_del(t_clean **lst)
{
	(*lst)->clean((*lst)->content);
	ft_free(&*lst);
}

void	lst_node_del_clean(t_clean **lst, void *mem)
{
	t_clean	*tmp;
	t_clean	*head;

	if (!lst || !*lst)
		return ;
	head = *lst;
	tmp = *lst;
	while ((*lst) && (*lst)->next->content != mem)
		(*lst) = (*lst)->next;
	if (!*lst)
		return ;
	tmp = *lst;
	if ((*lst)->next && (*lst)->next->next)
	{
		tmp = (*lst)->next->next;
		lst_node_del(&(*lst)->next);
		(*lst)->next = tmp;
	}
	else if ((*lst)->next)
		lst_node_del(&(*lst)->next);
	else
		lst_node_del(lst);
	*lst = head;
}

void	lst_list_clean(t_clean **head)
{
	t_clean	*tmp;

	while (*head)
	{
		tmp = (*head)->next;
		lst_node_del(head);
		*head = tmp;
	}
	free(*head);
}

int	lst_add_back(t_clean **node, t_clean *new)
{
	t_clean	*tmp;

	if (!new)
	{
		perror("malloc");
		return (0);
	}
	if (*node)
	{
		tmp = *node;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*node = new;
	return (1);
}
