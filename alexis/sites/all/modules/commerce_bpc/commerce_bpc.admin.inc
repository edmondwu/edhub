<?php
/**
 * @file
 * Page generation callbacks the Commerce Bulk Product Creation settings
 * pages.
 */

/**
 * Form callback for the settings page.
 */
function commerce_bpc_pattern_settings_form() {
  $form['default_patterns'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default patterns'),
    '#description' => t('Specify the patterns used for the generation of SKUs and titles during bulk creation.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('administer commerce_bpc'),
    '#tree' => FALSE,
  );

  $form['default_patterns']['commerce_bpc_default_sku_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU Pattern'),
    '#description' => t('Enter the pattern that should be used to generate SKUs. You should use the patterns below.'),
    '#size' => 160,
    '#maxlength' => 255,
    '#default_value' => variable_get('commerce_bpc_default_sku_pattern', '[bulk_defaults:entered_sku]--[bulk_defaults:combination_values]'),
  );

  $form['default_patterns']['commerce_bpc_default_sku_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator for combination field values in SKU'),
    '#description' => t('This string will be used to separate the values of various combination field. Thus if there are two combination fields "size" and "color", and for a particular combination, those have the values "M" and "red", respectively, with a separator value of "-", the string returned by [bulk_defaults:sku_combination_values] will be "M-red".'),
    '#size' => 3,
    '#maxlength' => 255,
    '#default_value' => variable_get('commerce_bpc_default_sku_separator', '-'),
  );

  $form['default_patterns']['commerce_bpc_default_title_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Title pattern'),
    '#description' => t('Enter the pattern that should be used to generate titles. You should use the patterns below.'),
    '#size' => 160,
    '#maxlength' => 255,
    '#default_value' => variable_get('commerce_bpc_default_title_pattern', '[bulk_defaults:entered_title] ([bulk_defaults:combination_labels])'),
  );

  $form['default_patterns']['commerce_bpc_default_title_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator for combination field labels in titles'),
    '#description' => t('This string will be used to separate the value-labels of various combination fields. Thus if there are two combination fields "size" and "color", and for a particular combination, those have the values "Medium" and "red", respectively, with a separator value of ", ", the string returned by [commerce_bpc:default_title_combination_values] will be "Medium, red".'),
    '#size' => 3,
    '#maxlength' => 255,
    '#default_value' => variable_get('commerce_bpc_default_title_separator', ', '),
  );
  $bpc_tokens = commerce_bpc_token_info();
  $form['tokens'] = _commerce_bpc_show_tokens('bulk_defaults', $bpc_tokens['tokens']['bulk_defaults']);

  return system_settings_form($form);
}
/**
 * Form callback for the settings page dealing with display node-related
 * settings.
 */
function commerce_bpc_display_node_settings_form() {
  $form = array();

  $form['display_creation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display node creation'),
    '#description' => t('How should the creation of display nodes be handled?'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('administer commerce_bpc'),
  );

  $form['display_creation']['commerce_bpc_display_creation_method'] = array(
    '#type' => 'radios',
    '#title' => t('Display node creation'),
    '#title_display' => 'invisible',
    '#description' => t('<strong>Note:</strong> The rest of the settings available on this page depends on what you select here.'),
    '#options' => array(
      'wizard' => t('Provide a "Save and create display" link on the Bulk creation form that takes the user to a pre-populated node creation form.'),
      'auto' => t('Silently create a display node automatically that references all the created products.'),
      'none' => t('Do not include any display node functionality.'),
    ),
    '#default_value' => variable_get('commerce_bpc_display_creation_method', 'wizard'),
  );


  $form['auto_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automatically created display nodes'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('administer commerce_bpc'),
    '#states' => array(
      'visible' => array(
        ':input[name="commerce_bpc_display_creation_method"]' => array('value' => 'auto'),
      ),
    ),
  );
  $required_if_auto = array(
    'required' => array(
      ':input[name="commerce_bpc_display_creation_method"]' => array('value' => 'auto'),
    )
  );
  $node_types = commerce_bpc_get_node_types();
  if (!empty($node_types)) {
    $form['auto_settings']['commerce_bpc_display_auto_content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content type of the created display node'),
      '#multiple' => FALSE,
      '#options' => $node_types,
      '#default_value' => variable_get('commerce_bpc_display_auto_content_type', NULL),
      '#states' => $required_if_auto,
    );
  }
  else {
    $form['auto_settings']['commerce_bpc_display_auto_content_type'] = array(
      '#markup' => '<p><strong>' . t('In order to auto-generate display nodes for all product types, you must create at least one content type that has a product reference field that can reference all product types. Visit the <a href="@content_type_path">content type page</a> to add such a field to a content type.',
      array(
        '@content_type_path' => '/admin/structure/types',
      )
      ) . '</strong></p>',
    );
    $form['auto_settings']['no_content_types_available'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
  }
  $form['auto_settings']['commerce_bpc_display_auto_node_title_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern for the title of the created display node'),
    '#description' => t('You can use the token %title_fragment_token for the title fragment entered by the user on the bulk creation form.', array(
           '%title_fragment_token' => '[bulk_display:title_fragment]'
    )),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => variable_get('commerce_bpc_display_auto_node_title_pattern', '[bulk_display:title_fragment]'),
    '#states' => $required_if_auto,
  );


  $form['auto_settings']['commerce_bpc_display_auto_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('After successful bulk creation, send the user to ...'),
    '#options' => array(
      'product listing' => t('... the list of products.'),
      'display node'    => t('... the newly created display node.'),
      'custom'          => t('... a custom location.'),
    ),
    '#default_value' => variable_get('commerce_bpc_display_auto_redirect', 'product_listing'),
  );
  $form['auto_settings']['redirection_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom redirect path'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#access' => user_access('administer commerce_bpc'),
    '#states' => array(
      'visible' => array(
        ':input[name="commerce_bpc_display_auto_redirect"]' => array('value' => 'custom'),
      ),
    ),
  );


$form['auto_settings']['redirection_settings']['commerce_bpc_display_auto_redirect_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom redirect path'),
    '#size' => 80,
    '#maxlength' => 255,
    '#title_display' => 'invisible',
    '#description' => 'Enter any valid drupal path.',
    '#default_value' => variable_get('commerce_bpc_display_auto_redirect_custom', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="commerce_bpc_display_auto_redirect"]' => array('value' => 'custom'),
      ),
      'required' => array(
        ':input[name="commerce_bpc_display_auto_redirect"]' => array('value' => 'custom'),
      ),
    ),
  );

  return system_settings_form($form);
}
function commerce_bpc_display_node_settings_form_validate($form, &$form_state) {
  $fv = &$form_state['values'];
  switch ($fv['commerce_bpc_display_creation_method']) {
    case 'auto':
      // Notify the user if there is no content type available.
      if (!empty($fv['no_content_types_available'])) {
        form_set_error('commerce_bpc_display_auto_content_type', 'You cannot activate auto-creation of display nodes if there are no appropriate display node types. See below for details.');
      }
      // Make sure all the required fields have values.
      if (empty($fv['commerce_bpc_display_auto_content_type'])) {
        form_set_error('commerce_bpc_display_auto_content_type', 'You have to specify a content type to automatically create.');
      }
      if (empty($fv['commerce_bpc_display_auto_node_title_pattern'])) {
        form_set_error('commerce_bpc_display_auto_node_title_pattern', 'For auto-creation of display nodes to work, you have to specify a pattern for the node title.');
      }
      if ($fv['commerce_bpc_display_auto_redirect'] == 'custom') {
        if (empty($fv['commerce_bpc_display_auto_redirect_custom'])) {
          form_set_error('commerce_bpc_display_auto_redirect', 'Please specify a custom redirect location');
        }
        elseif (!drupal_valid_path($fv['commerce_bpc_display_auto_redirect_custom'])) {
          form_set_error('commerce_bpc_display_auto_redirect_custom', 'Please specify a valid path as custom redirect location');
        }
      }
      break;
  }
}